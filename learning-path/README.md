# C++ Server Learning Path (From Wheel-Building to Workflow)

## Goal
Build your own tiny server framework from scratch, then map every piece to Workflow.

## Rules
- Do not jump to large framework code at the beginning.
- Each milestone must pass a clear checkpoint before moving on.
- At each step, implement core logic by yourself first.
- After each milestone, compare with Workflow source snippets.

## Milestones
1. M1: Blocking Echo Server (single connection, single thread)
2. M2: Non-blocking + epoll (single thread, multiple connections)
3. M3: EventLoop encapsulation + Reactor pattern
4. M4: ThreadPool + TaskQueue + producer/consumer
5. M5: Series task flow (mini workflow engine)
6. M6: Compare and evolve to Workflow architecture

## Workflow Mapping (preview)
- M1/M2 => networking basics behind kernel + server modules
- M3 => event-driven scheduling model
- M4 => executor/thread model
- M5 => task abstraction similar to series/task composition
- M6 => source-level optimization comparison
