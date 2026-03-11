#
#
#   This setups everything for the Datingapp.
#
#
#



import sqlite3
import uuid
# 1. Connect to the database (creates 'date.db' if it doesn't exist)
conn = sqlite3.connect('date.db')
cursor = conn.cursor()

# 2. Enable foreign keys
cursor.execute("PRAGMA foreign_keys = ON;")

# 3. Create the tables
cursor.executescript("""
    CREATE TABLE IF NOT EXISTS reviews (
        uuid TEXT PRIMARY KEY,
        score INTEGER
    );
    CREATE TABLE IF NOT EXISTS people (
        uuid TEXT PRIMARY KEY,
        name TEXT,
        age INTEGER,
        public_id TEXT,
        gender TEXT,
        gender_preferences TEXT,
        bio TEXT,
        age_preference_min INTEGER,
        age_preference_max INTEGER,
    );
    CREATE TABLE IF NOT EXISTS matches (
        id INTEGER PRIMARY KEY AUTOINCREMENT,
        matcher TEXT NOT NULL,
        matchee TEXT NOT NULL,
        ranking INTEGER,
        score INTEGER,
        review TEXT,
        FOREIGN KEY (matcher) REFERENCES people(uuid) ON DELETE CASCADE,
        FOREIGN KEY (matchee) REFERENCES people(uuid) ON DELETE CASCADE,
        FOREIGN KEY (review) REFERENCES reviews(uuid) ON DELETE SET NULL
    );
""")

c_uuid = uuid.uuid4()
cursor.execute(
    "INSERT INTO people (uuid, name, age, public_id) VALUES (?, ?, ?, ?)",
    (c_uuid, "Alice", 28, "apple-tree-fox")
)

# 5. Save and close
conn.commit()
conn.close()

print(f"Database setup complete. Inserted Alice with UUID: {c_uuid}")