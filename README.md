The taskmanagement system holds mainly two variables other than the counters of assignees and tasks; A red black tree hosting tasks and one hosting assignees.

The assignee and task header files host a struct of assignee and task nodes respectively and use these nodes to generate a tree based on the red black model.

A red black tree helps in indexing the assignees and their tasks in a balanced way so that any node can be searched an equally fast rate.
