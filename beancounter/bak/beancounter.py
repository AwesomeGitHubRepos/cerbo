import atexit

import psycopg2

def close_db(db):
    print("Closing database")
    db.close()

def open_db():
    """Open the database, with autoclose at exit"""
    connstr = "dbname = 'beancounter'"
    db = psycopg2.connect(connstr)
    atexit.register(close_db, db)
    return db
