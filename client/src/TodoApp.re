module TodoItem = {
  [@react.component]
  let make = (~task: TaskData.task, ~onToggle) => {
    <div className="item" onClick={_evt => onToggle()}>
      <input
        type_="checkbox"
        checked={task.completed}
        readOnly=true
        /* TODO make interactive */
      />
      {React.string(task.title)}
    </div>;
  };
};

let valueFromEvent = (evt): string => evt->ReactEvent.Form.target##value;
module Input = {
  type state = string;
  [@react.component]
  let make = (~onSubmit) => {
    let (text, setText) = React.useReducer((_, newText) => newText, "");
    <input
      value=text
      type_="text"
      placeholder="Write something to do"
      onChange={evt => setText(valueFromEvent(evt))}
      onKeyDown={evt =>
        if (ReactEvent.Keyboard.key(evt) == "Enter") {
          onSubmit(text);
          setText("");
        }
      }
    />;
  };
};

type state = {
  tasks: TaskData.tasks,
  loading: bool,
};

type action =
  | Loading
  | Loaded(TaskData.tasks)
  | AddItem(string)
  | ToggleItem(int);

let lastId = ref(0);
let newItem: string => TaskData.task =
  text => {
    lastId := lastId^ + 1;
    {id: lastId^ + 1, title: text, completed: true, uuid: "_"};
  };

let initialState = {tasks: [], loading: false};

[@react.component]
let make = () => {
  let ({tasks}, dispatch) =
    React.useReducer(
      (state, action) => {
        switch (action) {
        | Loading => {...state, loading: true}
        | Loaded(tasks) => {
            ...state,
            tasks: List.concat([state.tasks, tasks]),
          }
        | AddItem(text) => {
            ...state,
            tasks: [newItem(text), ...state.tasks],
          }
        | ToggleItem(id) => {
            ...state,
            tasks:
              List.map(
                (task: TaskData.task) =>
                  task.id === id
                    ? {...task, completed: !task.completed} : task,
                state.tasks,
              ),
          }
        }
      },
      initialState,
    );
  React.useEffect0(() => {
    TaskData.fetchTasks(payload => dispatch(Loaded(payload))) |> ignore;
    dispatch(Loading);
    None;
  });

  <div className="app">
    <div className="title">
      {React.string("What to do")}
      <Input onSubmit={text => dispatch(AddItem(text))} />
    </div>
    <div className="tasks">
      {List.map(
         (task: TaskData.task) =>
           <TodoItem
             key={string_of_int(task.id)}
             onToggle={() => dispatch(ToggleItem(task.id))}
             task
           />,
         tasks,
       )
       |> Array.of_list
       |> React.array}
    </div>
    <div className="footer">
      {(tasks->List.length->string_of_int ++ " tasks")->React.string}
    </div>
  </div>;
};