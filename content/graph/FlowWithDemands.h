/**
 * Author: unknown
 * Description: To solve flow with demands $d(e) \le f(e) \le c(e)$ for all edges e, make the following changes in the network: add a new source $s'$ and a new sink $t'$, a new edge from the source $s'$ to every other vertex, a new edge for every vertex to the sink $t'$, and one edge from $t$ to $s$. Additionally we define the new capacity function $c'$ as: $c'((s', v)) = \sum_{w \in V} d((u, v))$ for each edge $(s', v)$, $c'((v, t')) = \sum_{w \in V} d((v, w))$ for each edge $(v, t')$, $c'((u, v)) = c((u, v)) - d((u, v))$ for each edge $(u, v)$ in the old network, $c'((t, s)) = \infty$.
 * Solution exists if there's a saturating flow (every edge from $s'$ or to $t'$ is completely filled). To trace, for each edge $e$ in the old network, its flow is the flow of the corresponding edge $e'$ plus $d(e)$.
 * To find minimum flow, binary search the minimum capacity of the edge $(s, t)$ for saturating flow.
 * For node demands, connect a new source to all supply nodes with capacity supply $s(u)$, and a new sink to all demand nodes with capacity demand $d(v)$. 
 * If there's both node demands and edge demands, set each edge to lower bound. For each node $u$, define $L(u) = in(u) - out(u)$. Subtract $L(u)$ from supply/demand of $u$, problem transforms to node demands.
 * Time: dependent on flow algorithm
 */