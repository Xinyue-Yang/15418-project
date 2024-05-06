# Parallel maximum flow

## Project information

- Project name: parallel maximum flow
- Team members: Siyuan Chen, Xinyue Yang

## Summary

We parallelized two maximum flow algorithms
(Edmonds-Karp and Dinic's)
under the shared address space model using OpenMP.
We evaluated their performance on GHC and PSC machines against different network
types.
We demonstrated that (1) across the two algorithms,
the former is more parallelizable but overall the latter is more performant;
and (2) within each algorithm,
the top-down and bottom-up parallelism approaches are more suitable for sparse
and dense networks,
respectively.

<a href="docs/report.pdf" class="image fit">
    <img src="docs/poster.pdf" alt="poster">
</a>
