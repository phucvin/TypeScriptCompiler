type Record<K extends string | number | symbol, T> = { [P in K]: T; };
type Partial<T> = { [P in keyof T]?: T[P] | undefined; }
type Required<T> = { [P in keyof T]-?: T[P]; }
type Readonly<T> = { readonly [P in keyof T]: T[P]; }
type Pick<T, K extends keyof T> = { [P in K]: T[P]; }
type Omit<T, K extends string | number | symbol> = { [P in Exclude<keyof T, K>]: T[P]; }

interface CatInfo {
    age: number;
    breed: string;
}

type CatName = "miffy" | "boris" | "mordred";

interface Todo {
    title: string;
    description: string;
}

interface Props {
    a?: number;
    b?: string;
}

function updateTodo(todo: Todo, fieldsToUpdate: Partial<Todo>) {
    return { /*...todo,*/ ...fieldsToUpdate };
}

interface TodoPick {
    title: string;
    description: string;
    completed: boolean;
}

type TodoPreviewPick = Pick<TodoPick, "title" | "completed">;


interface TodoOmit {
    title: string;
    description: string;
    completed: boolean;
    createdAt: number;
}

type TodoPreviewOmit = Omit<TodoOmit, "description">;

function main() {
    const cats: Record<CatName, CatInfo> = {
        miffy: { age: 10, breed: "Persian" },
        boris: { age: 5, breed: "Maine Coon" },
        mordred: { age: 16, breed: "British Shorthair" },
    };

    print(cats.boris.breed);

    const todo1 = {
        title: "organize desk",
        description: "clear clutter",
    };

    const todo2 = updateTodo(todo1, {
        description: "throw out trash",
    });

    assert(todo2.description === "throw out trash");

    const obj: Props = { a: 5 };

    const obj2: Required<Props> = { a: 5 };

    print(obj.a, obj2.a);

    const todo: Readonly<Todo> = {
        title: "Delete inactive users",
    };

    print(todo.title);

    const todoPick: TodoPreviewPick = {
        title: "Clean room",
        completed: false,
    };

    const todoOmit: TodoPreviewOmit = {
        title: "Clean room",
        completed: false,
        createdAt: 1615544252770,
    };

    print("done.");
}