# XLSX Reader

This module reads XLSX files exported from Chilean banks and updates a SQL database. It's part of a bigger project called **Budget Monitor**.

## Dependencies

- **Qt Framework**: 6.8.3 or higher
- **C++**: 17 or higher
- [QXlsx](https://github.com/QtExcel/QXlsx.git): Automatically fetched during build

On Ubuntu/Debian, you'll also need:

```sh
sudo apt install libxkbcommon-dev
```

### Dev Notes: Why uploadDataToDataBase was removed
This method turned out to be a bad idea for a reusable module. Here's why:

A module should be general-purpose, but this function depended on how your specific database table was structured — not very flexible!

I considered passing the SQL query as a parameter, but since I'm using foreign keys, things got tricky. Sure, you can write queries with subqueries for foreign keys, but they don't handle "key not found" errors gracefully.

So I decided to get rid of it.

🪦RIP  *bool uploadDataToDataBase(QSqlDatabase db, const QString& nameTable)*