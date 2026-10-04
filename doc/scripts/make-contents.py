#!/usr/bin/env python3
"""Writes a browsable table of contents of the notebook as one HTML page.

Lists every printed entry with its page, description, complexity, printed
length and stress-test coverage, then the files that are in the repo but not
printed, then the techniques appendix. Run after `make kactl`, which writes
the page numbers this reads from build/kactl.ptc:

    python3 doc/scripts/make-contents.py site/contents.html
"""
import datetime, html, os, re, subprocess, sys
from collections import defaultdict

ROOT = os.path.normpath(os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', '..'))
CONTENT = os.path.join(ROOT, 'content')
REPO = os.environ.get('GITHUB_REPOSITORY', 'buidangnguyen05/kactl')
SOURCE = f'https://github.com/{REPO}/blob/main/'
CODE_EXT = ('.h', '.cpp', '.py', '.java', '.sh', '.txt')
PDF_OFFSET = 1  # printed page numbers start after the unnumbered cover

def read(path):
    with open(path, encoding='utf-8', errors='replace') as f:
        return f.read()

# ---------------------------------------------------------------- LaTeX -> HTML

def wrap_bigo(s):
    """O(...) with balanced parentheses -> $\\mathcal{O}(...)$, as the PDF does."""
    out, i = [], 0
    while (j := s.find('O(', i)) >= 0:
        if j > 0 and (s[j-1].isalnum() or s[j-1] == '\\'):
            out.append(s[i:j+2]); i = j + 2; continue
        depth, k = 1, j + 2
        while k < len(s) and depth:
            depth += {'(': 1, ')': -1}.get(s[k], 0); k += 1
        if depth:
            break
        out.append(s[i:j] + '$\\mathcal{O}(' + s[j+2:k-1] + ')$'); i = k
    return ''.join(out) + s[i:]

FMT = {'texttt': ('<code>', '</code>'), 'textbf': ('<strong>', '</strong>'), 'emph': ('<em>', '</em>'),
       'textit': ('<em>', '</em>'), 'textrm': ('', ''), 'text': ('', ''), 'mbox': ('', '')}

def strip_layout(s):
    """Drop PDF-only layout: figures, spacing, minipages, list environments."""
    s = re.sub(r'\\includegraphics(\[[^\]]*\])?\{[^}]*\}', '(figure in the PDF)', s)
    s = re.sub(r'\\vspace\*?\{[^}]*\}', ' ', s)
    s = re.sub(r'\\(begin|end)\{(minipage|itemize\*?|enumerate\*?|center|flushleft)\}(\{[^}]*\})?', ' ', s)
    return re.sub(r'(\\begin\{(align\*?|gather\*?|equation\*?)\}.*?\\end\{\2\})', r'$$\1$$', s)

def pre_format(s):
    r"""Text-mode \texttt{..}, \emph{..}, ... -> \x01name\x02 .. \x01/name\x02 markers, matching braces
    even across math; other grouping braces in text are dropped, macro arguments kept."""
    out, stack, depth, i, math = [], [], 0, 0, None
    while i < len(s):
        esc = i > 0 and s[i-1] == '\\'
        if math:
            if s.startswith(math, i) and not (math == '$' and esc):
                out.append(math); i += len(math); math = None
            else:
                out.append(s[i]); i += 1
            continue
        if s.startswith('$$', i) and not esc:
            math = '$$'; out.append('$$'); i += 2; continue
        if s[i] == '$' and not esc:
            math = '$'; out.append('$'); i += 1; continue
        if s.startswith('\\[', i) and not esc:
            math = '\\]'; out.append('\\['); i += 2; continue
        m = re.match(r'\\(' + '|'.join(FMT) + r')\{', s[i:])
        if m and not esc:
            stack.append(('fmt', m.group(1), depth)); depth += 1
            out.append('\x01' + m.group(1) + '\x02'); i += m.end(); continue
        if s[i] == '{' and not esc:
            keep = re.search(r'\\[A-Za-z]+$', ''.join(out[-20:])) is not None
            stack.append(('keep' if keep else 'drop', '', depth)); depth += 1
            if keep: out.append('{')
            i += 1; continue
        if s[i] == '}' and not esc and stack:
            depth -= 1
            kind, name, _ = stack.pop()
            out.append('\x01/' + name + '\x02' if kind == 'fmt' else '}' if kind == 'keep' else '')
            i += 1; continue
        out.append(s[i]); i += 1
    return ''.join(out)

def text_to_html(t):
    t = html.escape(t, quote=False)
    t = t.replace('\\item', '<br>&bull;')
    t = t.replace('\\\\', ' ').replace('---', '&mdash;').replace('--', '&ndash;')
    t = t.replace('``', '&ldquo;').replace("''", '&rdquo;').replace('~', '&nbsp;')
    t = re.sub(r'\\tilde(\{\})?', '~', t)
    t = re.sub(r'\\([_&#%{}])', r'\1', t)
    t = t.replace('\\$', '<span class="nomath">$</span>')
    # leftover macros outside math (\log, \sqrt{n}, ...): let KaTeX draw them
    return re.sub(r'\\[A-Za-z]+(?:\{[^{}]*\})*', lambda m: '$' + m.group() + '$', t)

def split_math(s):
    parts, pos = [], 0
    for m in re.finditer(r'\$\$.*?\$\$|(?<!\\)\$.*?(?<!\\)\$|\\\[.*?\\\]', s):
        parts.append(('t', s[pos:m.start()])); parts.append(('m', m.group())); pos = m.end()
    parts.append(('t', s[pos:]))
    return parts

def tex_to_html(s, bigo=False):
    s = pre_format(strip_layout(' '.join(s.split())))
    out = []
    for kind, seg in split_math(s):
        if kind == 'm':
            out.append(html.escape(seg, quote=False))
        elif bigo and 'O(' in seg:
            out.extend(html.escape(x, quote=False) if k == 'm' else text_to_html(x) for k, x in split_math(wrap_bigo(seg)))
        else:
            out.append(text_to_html(seg))
    res = ''.join(out)
    for name, (o, c) in FMT.items():
        res = res.replace('\x01' + name + '\x02', o).replace('\x01/' + name + '\x02', c)
    return res.strip()

# ---------------------------------------------------------------- sources

def header_fields(path):
    """The /** ... */ header of a notebook file as {field: text}."""
    m = re.search(r'/\*\*(.*?)\*/', read(path), re.S)
    fields, cur = {}, None
    for line in (m.group(1).split('\n') if m else []):
        line = re.sub(r'^\s*\*\s?', '', line)
        f = re.match(r'([A-Z][a-z]+):\s*(.*)', line)
        if f:
            cur = f.group(1); fields[cur] = f.group(2)
        elif cur:
            fields[cur] += ' ' + line.strip()
    return {k: v.strip() for k, v in fields.items()}

def printed_lines(path, opts):
    cmd = ['python3', os.path.join(CONTENT, 'tex', 'preprocessor.py'), '-i', path, '-o', '/dev/stdout'] + opts.split()
    out = subprocess.run(cmd, capture_output=True, text=True, cwd=ROOT).stdout
    m = re.search(r'(\d+) lines\}', out)
    return int(m.group(1)) if m else None

def includes(path):
    base = os.path.dirname(path)
    return {os.path.normpath(os.path.join(base, inc)) for inc in re.findall(r'#include\s+"([^"]+)"', read(path))}

chapters = re.findall(r'^[^%\n]*\\kactlchapter\{([^}]*)\}', read(os.path.join(CONTENT, 'kactl.tex')), re.M)
imports = {}            # (chapter dir, name) -> (path, preprocessor options)
commented = set()       # paths imported only in a commented-out line
printed = set()
for d in chapters:
    for line in read(os.path.join(CONTENT, d, 'chapter.tex')).split('\n'):
        m = re.match(r'^\s*(%?)\s*\\kactlimport(?:\[([^\]]*)\])?\{([^}]*)\}', line)
        if not m:
            continue
        path = os.path.normpath(os.path.join(CONTENT, d, m.group(3)))
        if m.group(1):
            commented.add(path)
        else:
            imports[(d, os.path.basename(path))] = (path, m.group(2) or '')
            printed.add(path)

# Stress-test coverage: headers a test includes directly, then anything those include.
direct, via = set(), {}
for dp, _, files in os.walk(os.path.join(ROOT, 'stress-tests')):
    for f in files:
        if f.endswith('.cpp'):
            direct |= {p for p in includes(os.path.join(dp, f)) if p.startswith(CONTENT)}
frontier = list(direct)
while frontier:
    h = frontier.pop()
    if not os.path.exists(h):
        continue
    for inc in includes(h):
        if inc.startswith(CONTENT) and inc not in direct and inc not in via:
            via[inc] = h; frontier.append(inc)

# ---------------------------------------------------------------- page numbers

ptc = os.path.join(ROOT, 'build', 'kactl.ptc')
log = read(os.path.join(ROOT, 'build', 'kactl.log'))
m = re.findall(r'Output written on .* \((\d+) pages?', log)
total_pages = int(m[-1]) if m else None

rows, ch = [], -1      # (kind, level, chapter index, text/name, number, page)
for line in read(ptc).split('\n'):
    m = re.match(r'^\\contentsline \{(\w+)\}\{(.*)\}\{(\d+)\}\{[^{}]*\}%?$', line)
    if not m:
        continue
    kind, title, page = m.group(1), m.group(2), int(m.group(3))
    level = {'chapter': 0, 'section': 1, 'subsection': 2, 'subsubsection': 3}[kind]
    if kind == 'chapter':
        ch += 1
    code = re.match(r'\\texttt \{(.*?)\}\\hspace', title)
    num = re.match(r'\\numberline \{([^}]*)\}(.*)', title)
    if code:
        rows.append(('entry', level, ch, code.group(1).replace('\\_', '_'), None, page))
    elif num:
        rows.append(('chapter' if kind == 'chapter' else 'heading', level, ch, num.group(2), num.group(1), page))
    elif kind != 'chapter':  # unnumbered heading, e.g. the distributions under probability
        rows.append(('heading', level, ch, title, '', page))

# ---------------------------------------------------------------- page

def page_link(p, label=None):
    return f'<a class="page" href="kactl.pdf#page={p + PDF_OFFSET}" title="Open page {p} of the PDF">{label or p}</a>'

def coverage(path):
    if path in direct:
        return 'tested', '<span class="ok">stress-tested</span>'
    if path in via:
        return 'tested', f'<span class="via">covered via {html.escape(os.path.basename(via[path]))}</span>'
    if path.endswith('.h'):
        return 'untested', '<span class="warn">no stress test</span>'
    return 'n/a', ''

def entry_html(name, path, opts, page=None):
    f = header_fields(path) if path.endswith(('.h', '.cpp', '.java', '.py')) else {}
    status, cov = coverage(path)
    facts = []
    if f.get('Time'):
        facts.append('<span class="fact">Time ' + tex_to_html(f['Time'], bigo=True) + '</span>')
    if f.get('Memory'):
        facts.append('<span class="fact">Memory ' + tex_to_html(f['Memory'], bigo=True) + '</span>')
    n = printed_lines(path, opts)
    if n:
        facts.append(f'<span class="fact">{n} line{"s" * (n != 1)}</span>')
    if cov:
        facts.append(cov)
    rel = os.path.relpath(path, ROOT)
    desc = tex_to_html(f['Description']) if f.get('Description') else ''
    right = '<span class="leader" aria-hidden="true"></span>' + page_link(page) if page is not None else ''
    return (f'<li class="entry" data-status="{status}">'
            f'<div class="line"><a class="name" href="{SOURCE}{rel}">{html.escape(name)}</a>{right}</div>'
            + (f'<p class="desc">{desc}</p>' if desc else '')
            + (f'<p class="facts">{" ".join(facts)}</p>' if facts else '') + '</li>')

chapter_names, chapter_pages = {}, {}
for kind, level, c, text, num, page in rows:
    if kind == 'chapter':
        chapter_names[c] = tex_to_html(text); chapter_pages[c] = page

body, n_entries, n_tested, n_untested, chapter_range = [], 0, 0, 0, {}
for c in sorted(chapter_names):
    first = chapter_pages[c]  # a chapter ends on the page where the next one starts
    last = chapter_pages[c + 1] if c + 1 in chapter_pages else (total_pages - PDF_OFFSET - 1 if total_pages else first)
    chapter_range[c] = f'{first}&ndash;{last}' if last > first else f'{first}'
    label = f'pages {first}&ndash;{last}' if last > first else f'page {first}'
    body.append(f'<section class="chapter" id="ch-{c+1}"><h2><span class="num">{c+1}</span>{chapter_names[c]}'
                f'<span class="span">{page_link(first, label)}</span></h2><ul class="toc">')
    for kind, level, cc, text, num, page in rows:
        if cc != c or kind == 'chapter':
            continue
        if kind == 'heading':
            secnum = f'<span class="secnum">{html.escape(num)}</span>' if num else ''
            body.append(f'<li class="heading level-{level}"><div class="line"><span class="title">{secnum}'
                        f'{tex_to_html(text)}</span><span class="leader" aria-hidden="true"></span>{page_link(page)}</div></li>')
        else:
            key = (chapters[c], text)
            if key not in imports:
                print(f'warning: {text} is in kactl.ptc but not imported in {chapters[c]}/chapter.tex', file=sys.stderr)
                continue
            path, opts = imports[key]
            body.append(entry_html(text, path, opts, page).replace('class="entry"', f'class="entry level-{level}"', 1))
            n_entries += 1
            s = coverage(path)[0]
            n_tested += s == 'tested'; n_untested += s == 'untested'
    body.append('</ul></section>')

# In the repo but not printed.
skip_dirs = {'tex', 'test-session'}
missing = defaultdict(list)
for d in sorted(os.listdir(CONTENT)):
    full = os.path.join(CONTENT, d)
    if not os.path.isdir(full) or d in skip_dirs:
        continue
    for f in sorted(os.listdir(full)):
        p = os.path.join(full, f)
        if f.endswith(CODE_EXT) and os.path.isfile(p) and p not in printed:
            missing[d].append(p)
n_missing = sum(len(v) for v in missing.values())
body.append('<section class="chapter extra" id="not-printed"><h2>In the repo, not printed</h2>'
            '<p class="note">These files exist in <code>content/</code> but are left out of the PDF, '
            'so they are one line in a <code>chapter.tex</code> away from being printed.</p>')
for d, paths in missing.items():
    body.append(f'<h3>{html.escape(d)}</h3><ul class="toc">')
    for p in paths:
        item = entry_html(os.path.basename(p), p, '-l raw' if p.endswith('.txt') else '')
        if p in commented:
            item = item.replace('<p class="facts">', '<p class="facts"><span class="fact">commented out in chapter.tex</span> ', 1)
        body.append(item.replace('class="entry"', 'class="entry np"', 1))
    body.append('</ul>')
body.append('</section>')

# Techniques appendix (names only).
tech = read(os.path.join(CONTENT, 'appendix', 'techniques.txt')).split('\n')
items = []
for line in tech:
    if not line.strip():
        continue
    depth = len(line) - len(line.lstrip('\t'))
    text = line.strip()
    if text.startswith('* '):
        depth += 1; text = text[2:]
    items.append((depth, text))
tl = []
prev = -1
for depth, text in items:
    if depth > prev:
        tl.append('<ul>' * (depth - prev))
    elif depth < prev:
        tl.append('</li>' + '</ul></li>' * (prev - depth))
    else:
        tl.append('</li>')
    tl.append(f'<li class="tech"><span>{html.escape(text)}</span>')
    prev = depth
tl.append('</li>' + '</ul></li>' * prev + '</ul>')
tech_page = (total_pages - PDF_OFFSET) if total_pages else None
body.append('<section class="chapter extra" id="techniques"><h2>Techniques checklist'
            + (f'<span class="span">{page_link(tech_page, f"page {tech_page}")}</span>' if tech_page else '') + '</h2>'
            '<p class="note">The appendix page: a list of technique names to think through when stuck. '
            'Names only, no code, so a match here is not an implementation.</p>'
            f'<details><summary>Show all {len(items)} techniques</summary>{"".join(tl)}</details></section>')

sha = os.environ.get('GITHUB_SHA') or subprocess.run(['git', 'rev-parse', 'HEAD'], capture_output=True, text=True, cwd=ROOT).stdout.strip()
built = datetime.datetime.now(datetime.timezone.utc).strftime('%Y-%m-%d %H:%M UTC')
rail = ''.join(f'<li><a href="#ch-{c+1}"><span class="rn">{c+1}</span><span class="rt">{chapter_names[c]}</span>'
               f'<span class="rp">{chapter_range[c]}</span></a></li>' for c in sorted(chapter_names))
rail += '<li class="sep"><a href="#not-printed">Not printed</a></li><li><a href="#techniques">Techniques</a></li>'

PAGE = r'''<!doctype html>
<html lang="en">
<head>
<meta charset="utf-8">
<meta name="viewport" content="width=device-width, initial-scale=1">
<title>Notebook contents</title>
<link rel="preconnect" href="https://fonts.googleapis.com">
<link rel="preconnect" href="https://fonts.gstatic.com" crossorigin>
<link href="https://fonts.googleapis.com/css2?family=JetBrains+Mono:wght@400;600&family=Literata:ital,opsz,wght@0,7..72,400;0,7..72,600;1,7..72,400&display=swap" rel="stylesheet">
<link rel="stylesheet" href="https://cdn.jsdelivr.net/npm/katex@0.16.11/dist/katex.min.css" integrity="sha384-nB0miv6/jRmo5UMMR1wu3Gz6NLsoTkbqJghGIsx//Rlm+ZU03BU6SQNC66uf4l5+" crossorigin="anonymous">
<script defer src="https://cdn.jsdelivr.net/npm/katex@0.16.11/dist/katex.min.js" integrity="sha384-7zkQWkzuo3B5mTepMUcHkMB5jZaolc2xDwL6VFqjFALcbeS9Ggm/Yr2r3Dy4lfFg" crossorigin="anonymous"></script>
<script defer src="https://cdn.jsdelivr.net/npm/katex@0.16.11/dist/contrib/auto-render.min.js" integrity="sha384-43gviWU0YVjaDtb/GhzOouOXtZMP/7XUzwPTstBeZFe/+rCMvRwr4yROQP43s0Xk" crossorigin="anonymous" onload="renderMath()"></script>
<style>
:root {
  --paper: #FDFDFB; --ink: #1E2023; --muted: #5E636B; --rule: #DCDDE0;
  --pen: #2350B8; --mark: #FFE45C; --ok: #2E7A4A; --warn: #A8322D;
  --serif: "Literata", Georgia, "Times New Roman", serif;
  --mono: "JetBrains Mono", ui-monospace, "SFMono-Regular", Menlo, Consolas, monospace;
}
@media (prefers-color-scheme: dark) {
  :root { --paper: #17191C; --ink: #E7E6E1; --muted: #9BA0A8; --rule: #34373C;
          --pen: #93B2FF; --mark: #F2D33F; --ok: #74C493; --warn: #F2918A; }
}
* { box-sizing: border-box; }
html { scroll-padding-top: 6.5rem; }
body { margin: 0; background: var(--paper); color: var(--ink); font: 17px/1.6 var(--serif); }
a { color: var(--pen); text-decoration: none; }
a:hover { text-decoration: underline; }
a:focus-visible, input[type=checkbox]:focus-visible, summary:focus-visible { outline: 2px solid var(--pen); outline-offset: 2px; border-radius: 2px; }
.search input[type=search]:focus-visible { outline: none; border-bottom-color: var(--pen); box-shadow: 0 1px 0 var(--pen); }
code { font: .86em var(--mono); }
.masthead { max-width: 74rem; margin: 0 auto; padding: 3rem 1.5rem 1.25rem; }
.masthead h1 { font-size: 2.6rem; line-height: 1.15; font-weight: 600; margin: 0 0 .6rem; letter-spacing: -.01em; }
.masthead .lede { max-width: 40rem; margin: 0 0 1rem; color: var(--muted); }
.masthead .links { display: flex; flex-wrap: wrap; gap: .4rem 1.5rem; margin: 0; padding: 0; list-style: none; }
.layout { max-width: 74rem; margin: 0 auto; padding: 0 1.5rem 4rem; display: grid; grid-template-columns: 14rem minmax(0, 1fr); gap: 3rem; }
.rail { position: sticky; top: 1rem; align-self: start; max-height: calc(100vh - 2rem); overflow: auto; padding-top: 5.2rem; }
.rail ol { list-style: none; margin: 0; padding: 0; }
.rail a { display: flex; gap: .6rem; padding: .22rem 0; color: var(--ink); font-size: .95rem; }
.rail a:hover { color: var(--pen); text-decoration: none; }
.rail .rn { width: 1.3rem; color: var(--muted); font-variant-numeric: tabular-nums; }
.rail .rt { flex: 1; }
.rail .rp { color: var(--muted); font-size: .85rem; font-variant-numeric: tabular-nums; }
.rail .sep { margin-top: .8rem; padding-top: .8rem; border-top: 1px solid var(--rule); }
.rail li.empty a { color: var(--muted); opacity: .55; }
.search { position: sticky; top: 0; z-index: 2; background: var(--paper); padding: 1rem 0 .9rem; border-bottom: 1px solid var(--rule); }
.search label.q { display: block; }
.search input[type=search] { width: 100%; font: 1.15rem var(--serif); color: var(--ink); background: transparent;
  border: 0; border-bottom: 2px solid var(--ink); padding: .35rem 0; border-radius: 0; }
.search input[type=search]::placeholder { color: var(--muted); }
.search .row { display: flex; flex-wrap: wrap; justify-content: space-between; gap: .3rem 1rem; margin-top: .55rem; font-size: .92rem; color: var(--muted); }
.search .row label { cursor: pointer; }
.chapter { margin-top: 2.6rem; }
.chapter h2 { display: flex; align-items: baseline; gap: .8rem; font-size: 1.55rem; line-height: 1.25; font-weight: 600; margin: 0 0 .6rem; }
.chapter h2 .num { color: var(--muted); font-weight: 400; font-variant-numeric: tabular-nums; }
.chapter h2 .span { margin-left: auto; font-size: .95rem; font-weight: 400; }
.chapter h3 { font: 600 .95rem var(--mono); margin: 1.6rem 0 .2rem; color: var(--muted); }
.note { color: var(--muted); max-width: 40rem; margin: 0 0 .5rem; }
.toc { list-style: none; margin: 0; padding: 0; }
.toc li { padding: .55rem 0; }
.line { display: flex; align-items: baseline; gap: .5rem; }
.leader { flex: 1; min-width: 1.5rem; border-bottom: 2px dotted var(--rule); transform: translateY(-.3em); }
.page { font-variant-numeric: tabular-nums; min-width: 1.6rem; text-align: right; }
.name { font: 600 .95rem var(--mono); color: var(--ink); overflow-wrap: anywhere; }
.name:hover { color: var(--pen); }
.entry.level-2 { padding-left: 1.4rem; }
.heading { padding: .9rem 0 .2rem; }
.heading .title { font-weight: 600; }
.heading.level-2 { padding-left: 1.4rem; }
.heading.level-2 .title, .heading.level-3 .title { font-weight: 400; font-style: italic; }
.heading.level-3 { padding-left: 2.8rem; }
.secnum { color: var(--muted); font-weight: 400; font-style: normal; margin-right: .6rem; font-variant-numeric: tabular-nums; }
.desc { margin: .2rem 0 0; max-width: 42rem; line-height: 1.62; }
.facts { margin: .25rem 0 0; font-size: .88rem; color: var(--muted); display: flex; flex-wrap: wrap; gap: .1rem 1.1rem; }
.facts .ok { color: var(--ok); }
.facts .warn { color: var(--warn); }
.entry.level-2 .desc, .entry.level-2 .facts { max-width: 40.6rem; }
.desc .katex-display { overflow-x: auto; overflow-y: hidden; padding: .2rem 0; }
mark { background: var(--mark); color: #1E2023; padding: 0 .08em; border-radius: 2px; }
.empty-state { margin: 2.5rem 0; max-width: 36rem; }
details summary { cursor: pointer; color: var(--pen); margin: .4rem 0; }
#techniques ul { list-style: none; padding-left: 1.3rem; margin: .1rem 0; }
#techniques details > ul { padding-left: 0; columns: 2 18rem; column-gap: 2.5rem; }
#techniques details > ul > li { break-inside: avoid; margin: 0 0 .7rem; }
#techniques details > ul > li > span { font-weight: 600; }
.tech { line-height: 1.45; }
footer { color: var(--muted); font-size: .88rem; margin-top: 4rem; padding-top: 1rem; border-top: 1px solid var(--rule); }
.hidden { display: none !important; }
.sr { position: absolute; width: 1px; height: 1px; overflow: hidden; clip: rect(0 0 0 0); white-space: nowrap; }
@media (max-width: 860px) {
  .layout { grid-template-columns: minmax(0, 1fr); gap: 0; }
  .rail { position: static; max-height: none; padding-top: 0; overflow-x: auto; border-bottom: 1px solid var(--rule); }
  .rail ol { display: flex; gap: 1.2rem; white-space: nowrap; padding-bottom: .6rem; }
  .rail .sep { margin-top: 0; padding-top: 0; border-top: 0; }
  .masthead h1 { font-size: 2rem; }
  .entry.level-2, .heading.level-2 { padding-left: .8rem; }
  .rail .rp { display: none; }
  .desc .katex { display: inline-block; max-width: 100%; overflow-x: auto; overflow-y: hidden; vertical-align: middle; }
  .chapter h2 { flex-wrap: wrap; }
}
@media (max-width: 480px) {
  body { font-size: 16px; }
  .masthead, .layout { padding-left: 1rem; padding-right: 1rem; }
}
</style>
</head>
<body>
<header class="masthead">
  <h1>What's in the notebook</h1>
  <p class="lede">Every entry in the printed notebook with its page, what it does, and whether a stress test checks it. @@SUMMARY@@</p>
  <ul class="links"><li><a href="kactl.pdf">Open the PDF</a></li><li><a href="test-session.pdf">Test session PDF</a></li><li><a href="https://github.com/@@REPO@@">Source on GitHub</a></li></ul>
</header>
<div class="layout">
<nav class="rail" aria-label="Chapters"><ol>@@RAIL@@</ol></nav>
<main>
<div class="search" role="search">
  <label class="q"><span class="sr">Search the notebook</span>
  <input id="q" type="search" placeholder="Search, e.g. LCA or segment tree" autocomplete="off" spellcheck="false"></label>
  <div class="row"><label><input id="untested" type="checkbox"> Only entries without a stress test</label><span id="count" aria-live="polite"></span></div>
</div>
<p class="empty-state hidden" id="empty"></p>
@@BODY@@
<footer>Built from <a href="https://github.com/@@REPO@@/commit/@@SHA@@">@@SHORT@@</a> on @@BUILT@@ by <code>doc/scripts/make-contents.py</code>, on every push to main.</footer>
</main>
</div>
<script>
function renderMath() {
  renderMathInElement(document.body, {
    delimiters: [{left: "$$", right: "$$", display: true}, {left: "\\[", right: "\\]", display: true}, {left: "$", right: "$", display: false}],
    throwOnError: false, ignoredClasses: ["nomath"]
  });
}
(function () {
  var q = document.getElementById("q"), untested = document.getElementById("untested");
  var count = document.getElementById("count"), empty = document.getElementById("empty");
  var rows = Array.prototype.slice.call(document.querySelectorAll(".toc > li, #techniques li.tech"));
  var chapters = Array.prototype.slice.call(document.querySelectorAll(".chapter"));
  var details = document.querySelector("#techniques details");
  function unmark(root) {
    root.querySelectorAll("mark").forEach(function (m) { m.replaceWith(document.createTextNode(m.textContent)); });
    root.normalize();
  }
  function mark(root, terms) {
    var walker = document.createTreeWalker(root, NodeFilter.SHOW_TEXT, {
      acceptNode: function (n) { return n.parentElement.closest(".katex, .page, .secnum") ? NodeFilter.FILTER_REJECT : NodeFilter.FILTER_ACCEPT; }
    });
    var nodes = [];
    while (walker.nextNode()) nodes.push(walker.currentNode);
    var re = new RegExp("(" + terms.map(function (t) { return t.replace(/[.*+?^${}()|[\]\\]/g, "\\$&"); }).join("|") + ")", "gi");
    nodes.forEach(function (n) {
      if (!re.test(n.nodeValue)) return;
      re.lastIndex = 0;
      var span = document.createElement("span");
      span.innerHTML = n.nodeValue.replace(/[&<>]/g, function (c) { return {"&": "&amp;", "<": "&lt;", ">": "&gt;"}[c]; }).replace(re, "<mark>$1</mark>");
      n.replaceWith.apply(n, Array.prototype.slice.call(span.childNodes));
    });
  }
  function own(li) { // a row's own text, without nested technique rows
    if (!li.classList.contains("tech")) return li.textContent;
    return li.firstElementChild ? li.firstElementChild.textContent : li.textContent;
  }
  function update() {
    var text = q.value.trim().toLowerCase(), terms = text.split(/\s+/).filter(Boolean), onlyUntested = untested.checked;
    var shown = 0, extra = 0;
    rows.forEach(function (li) {
      unmark(li);
      var hay = own(li).toLowerCase();
      var ok = terms.every(function (t) { return hay.indexOf(t) >= 0; });
      if (onlyUntested) ok = ok && li.dataset.status === "untested";
      if (!terms.length && !onlyUntested) ok = true;
      li.classList.toggle("hidden", !ok && !li.classList.contains("tech"));
      li.dataset.match = ok ? "1" : "";
      if (ok && terms.length) { mark(li.classList.contains("tech") ? li.firstElementChild : li, terms); }
      if (ok && li.classList.contains("entry") && (terms.length || onlyUntested)) { if (li.classList.contains("np")) extra++; else shown++; }
    });
    var filtering = terms.length > 0 || onlyUntested;
    // techniques: keep the tree, open it when a technique matches
    var techHit = rows.some(function (li) { return li.classList.contains("tech") && li.dataset.match && terms.length; });
    if (details) details.open = techHit || details.dataset.userOpen === "1";
    chapters.forEach(function (sec) {
      var any = sec.querySelector(".toc > li.entry:not(.hidden), .toc > li.heading:not(.hidden)") || (sec.id === "techniques" && (!filtering || techHit));
      sec.classList.toggle("hidden", filtering && !any);
      var link = document.querySelector('.rail a[href="#' + sec.id + '"]');
      if (link) link.parentElement.classList.toggle("empty", filtering && !any);
    });
    var parts = [];
    if (filtering) {
      parts.push(shown ? shown + (shown === 1 ? " printed entry" : " printed entries") : "No printed entries");
      if (extra) parts.push(extra + " more in the repo, not printed");
      if (techHit) parts.push("named in the techniques checklist");
    }
    count.textContent = parts.join("; ");
    if (filtering && !shown && !extra && !techHit) {
      empty.textContent = onlyUntested && !terms.length ? "Every printed entry has a stress test."
        : "Nothing matches \u201c" + q.value.trim() + "\u201d. The notebook may call it something else: try a shorter or different name.";
      empty.classList.remove("hidden");
    } else empty.classList.add("hidden");
  }
  if (details) details.addEventListener("toggle", function () { if (!q.value.trim()) details.dataset.userOpen = details.open ? "1" : ""; });
  q.addEventListener("input", update);
  untested.addEventListener("change", update);
  document.addEventListener("keydown", function (e) {
    if (e.key === "/" && document.activeElement !== q) { e.preventDefault(); q.focus(); }
    if (e.key === "Escape" && document.activeElement === q) { q.value = ""; update(); }
  });
})();
</script>
</body>
</html>
'''

summary = (f'{n_entries} entries over {total_pages} pages; '
           f'{n_tested} are covered by stress tests and {n_untested} are not. '
           f'{n_missing} more files are in the repo but not printed.')
out = PAGE
for tok, val in dict(summary=html.escape(summary), repo=REPO, rail=rail, body='\n'.join(body),
                     sha=sha, short=sha[:7], built=built).items():
    out = out.replace('@@' + tok.upper() + '@@', val)
dest = sys.argv[1] if len(sys.argv) > 1 else '-'
if dest == '-':
    sys.stdout.write(out)
else:
    with open(dest, 'w', encoding='utf-8') as f:
        f.write(out)
