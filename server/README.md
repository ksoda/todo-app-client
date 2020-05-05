# actix-todo

A port of the [Rocket Todo example](https://github.com/SergioBenitez/Rocket/tree/master/examples/todo) into [actix-web](https://actix.rs/). Except this uses PostgreSQL instead of SQLite.

# Usage

## Prerequisites

- Rust >= 1.26
- PostgreSQL >= 9.5

## Change into the project sub-directory

All instructions assume you have changed into this folder:

```bash
cd examples/todo
```

## Set up the database

Install the [diesel](http://diesel.rs) command-line tool including the `postgres` feature:

```bash
cargo install diesel_cli --no-default-features --features postgres
```

Check the contents of the `.env` file. If your database requires a password, update `DATABASE_URL` to be of the form:

```.env
DATABASE_URL=postgres://username:password@localhost/actix_todo
```

Start DB

```bash
docker run --net=host --name some-postgres -e POSTGRES_PASSWORD=secret -d postgres
```

Then to create and set-up the database run:

```bash
diesel database setup
```

## Run the application

To run the application execute:

```bash
cargo run
```

Then to view it in your browser navigate to: [http://localhost:8088/](http://localhost:8088/)

## Build the image

```bash
docker build -t todo .
```

Then to run the container:

```bash
docker run --net=host --rm -e DATABASE_URL=postgres://postgres:secret@localhost/todo todo
```

## Deploy to Heroku

To deploy the application:

<!-- TODO: Terraform -->

```bash
sudo snap install --classic heroku
heroku plugins:install @heroku-cli/plugin-manifest
name=$(ruby -rhaikunator -e 'puts Haikunator.haikunate')
heroku create $name --manifest
heroku addons:create heroku-postgresql:hobby-dev --app $name
heroku git:remote -a $name
heroku stack:set container
```