static constexpr auto pi = acos(-1);

typedef double ld;

struct point {
    ld x, y;
    point () {};
    point (ld _x, ld _y) {
        x = _x;
        y = _y;
    }
    bool operator == (const point &X) {return x == X.x && y == X.y;}
    bool operator < (const point &X) const {
        if (x != X.x)
            return x < X.x;
        return y < X.y;
    }
    point operator + (const point &X) {
        return point(x + X.x, y + X.y);
    }
    point operator - (const point &X) {
        return point(x - X.x, y - X.y);
    }
};

int ccw(point p, point q, point r) {
    ld val = (q.y - p.y) * (r.x - q.x) - (q.x - p.x) * (r.y - q.y);
    if (val == 0) return 0;
    return (val > 0) ? 1 : 2; // 1: clock, 2: counter
}

ld dist (const point &X, const point &Y) {
    return sqrt((X.x - Y.x) * (X.x - Y.x) + (X.y - Y.y) * (X.y - Y.y));
}

// Midpoint
point getMid(const point &X, const point &Y) {
    return point((X.x + Y.x) / 2, (X.y + Y.y) / 2);
}

#define printPoint(X) cerr << fixed << setprecision(12) << #X << ": " << X.x << " " << X.y << "\n"

struct line {
    ld a, b, c, m;
    line () {};
    line (ld _a, ld _b, ld _c) {
        a = _a; b = _b; c = _c;
        m = -a / b;
    }
    bool operator == (const line &X) {
        return a == X.a && b == X.b && c == X.c;
    }
    bool operator < (const line &X) const {
        if (a != X.a)
            return a < X.a;
        if (b != X.b)
            return b < X.b;
        return c < X.c;
    }
};

// Perpendicular bisector of line XY
line getPer(point X, point Y) {
    point mid = getMid(X, Y), v = Y - X;
    return line(v.x, v.y, -v.x * mid.x - v.y * mid.y);
}

// line XY
line getThrough(point X, point Y) {
    point v = point(-(Y - X).y, (Y - X).x);
    return line(v.x, v.y, -v.x * X.x - v.y * X.y);
}

// intersection of 2 lines
point intersection(line X, line Y) {
    return point((X.b * Y.c - Y.b * X.c) / (X.a * Y.b - X.b * Y.a), (X.c * Y.a - X.a * Y.c) / (X.a * Y.b - X.b * Y.a));
}

point get(line d, ld x) {
    return point(x, -(x * d.a + d.c) / d.b);
}

// calculate angle A knowing BC = a, AC = b, AB = c
ld angle(ld a, ld b, ld c) {
    return 180.0 / pi * acos((b * b + c * c - a * a) / (2 * b * c));
}

// calculate angle BAC
ld angle(point B, point A, point C) { 
    return angle(dist(B, C), dist(A, C), dist(A, B));
}

// get other vertex of equilateral triangle with edge AB
point getEquilateral(point A, point B, point C) {
    point m1 = getMid(A, B); line p1 = getPer(A, B);
    point u1, u2;
    ld d = sqrt(3.0) * dist(A, B) / 2;
    if (!p1.b) u1 = point(m1.x, m1.y + d), u2 = point(m1.x, m1.y - d);
    else {
        ld diff = sqrt(d * d / (1.0 + p1.m * p1.m));
        u1 = get(p1, m1.x + diff); u2 = get(p1, m1.x - diff);
    }
    if (ccw(A, C, B) == ccw(A, u1, B)) swap(u1, u2);
    return u1;
}

// Fermat point: Point that minimizes total distance to the 3 vertices
point minimize(point A, point B, point C) {
    if (A == B || A == C) return A; if (B == C) return B;
    if (angle(dist(B, C), dist(A, B), dist(A, C)) >= 120.0) return A;
    if (angle(dist(A, B), dist(B, C), dist(A, C)) >= 120.0) return C;
    if (angle(dist(A, C), dist(B, C), dist(A, B)) >= 120.0) return B;

    line p1 = getThrough(getEquilateral(A, B, C), C);
    line p2 = getThrough(getEquilateral(A, C, B), B);
    return intersection(p1, p2);
}

// rotate point p around point q with angle delta(degrees) counterclockwise.
point rotate(point p, point q, ld delta) {
    delta = delta * pi / 180;
    return point((p.x - q.x) * cos(delta) - (p.y - q.y) * sin(delta) + q.x, (p.x - q.x) * sin(delta) + (p.y - q.y) * cos(delta) + q.y);
}