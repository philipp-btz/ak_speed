# AK Speed
AK SPEED is a joint venture of the FS Informatik, Mathematics and Physics of the University Heidelberg




## Database Schema SQLite3
- Table: people
    - uuid : TEXT PRIMARY KEY (uuid7, wird autoatisch bei erstellung generiert)
    - name : TEXT
    - age : INTEGER
    - public_id : TEXT (identifier word, Pool an Wörtern TBD)
    - gender : TEXT (m/f/d)
    - gender_preference : TEXT (m/f/d)
    - bio : TEXT
    - age_preference_min : INTEGER
    - age_preference_max : INTEGER

- Table: matches
    - id : INTEGER PRIMARY KEY
    - matcher : TEXT (uuid of matched person A)
    - matchee : TEXT (uuid of matched person B)
    - ranking : INTEGER (der wie vielte Platz dieses Ranking bei dem Matcher hat)
    - score : INTEGER (der Score des matches)
    - review : TEXT (uuid of review)

- Tables: reviews
    - uuid : TEXT PRIMARY IDENTIFIER (uuid7)
    - score : INTEGER (angegeben vom user)