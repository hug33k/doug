from sqlmodel import SQLModel, create_engine, Session
from . import models
from . import dump


db = None


def get_dabatase(path="database/"):
    global db
    if (db is None):
        databaseFile = "db.sqlite"
        sqliteUrl = f"sqlite:///{path}{databaseFile}"
        db = create_engine(sqliteUrl, echo=True)
    return db


def get_session():
    db = get_dabatase()
    return Session(db)


def create_database_and_tables(engine):
    SQLModel.metadata.create_all(engine)


def init_db():
    engine = get_dabatase("")
    create_database_and_tables(engine)
    dump.inject(engine)
