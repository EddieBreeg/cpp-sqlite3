/* This code gives you an example of how one would use 
the C++ API, by emulating a simple app where users can log in
and create posts. This code is not in ANY CASE
meant to show you how to build such an application, and should 
therefore not be used in a production environment under ANY circumstances,
for obvious security reasons! 
 */

#include <SQLite3/SQLite3.hpp>
#include <iostream>
#include <string>

SQLite3::error_code printPost(const SQLite3::QueryResult& res){
    if(!res.columns()) return SQLite3::ABORT;
    std::cout << res.at<std::string_view>(0) /* author's name */ << " wrote: "
    << res.at<std::string_view>(1) /* content */ << '\n';
    return SQLite3::OK;
}
SQLite3::error_code fetchUserId(const SQLite3::QueryResult& res, int& id){
    if(!res.columns()) return SQLite3::ABORT;
    id = res.at<int32_t>(0);
    return SQLite3::OK;
}

SQLite3::error_code createPost(SQLite3::Database& db, int userId){
    std::string postContent;
    std::cout << "Wanna say something?\nPost: ";
    std::getline(std::cin, postContent);
    if(postContent.empty()) return SQLite3::DONE;
    auto st = db.createStatement("insert into posts (content, author_id) values (?, ?)");
    st.bindParams(postContent, userId);
    SQLite3::error_code ec;
    st(ec);
    return ec;
}

int main(int argc, char const *argv[])
{
    auto err = SQLite3::init();
    if(err){
        std::cerr << "Couldn't init SQLite3: " << err.what() << '\n';
        return 1;
    }
    SQLite3::Database db("main.db");
    if(!db.isOpen()){
        err = db.getError();
        std::cerr << err.what() << '\n';
        return 1;
    }

    db.toggleForeignKeys(true); // activate foreign key constraints
    // create the tables
    err = db.execute("create table users (id integer primary key not null,"
    "name varchar(50) unique not null,"
    "password varchar(128) not null"
    ");"
    "create table posts (id integer primary key not null,"
    "content text,"
    "author_id integer not null,"
    "foreign key(author_id) references users(id)"
    ")");
    if(err && err != SQLite3::ERROR /* Tables couldn't be created, but not because of a SQL error */){
        std::cerr << "Couldn't create tables: " << err.what() << '\n';
        return 1;
    }
    std::string username, pwd;
    do{
        std::cout << "Name: ";
        std::getline(std::cin, username);
    } while(username.empty());
    do
    {
        std::cout << "Password: ";
        std::getline(std::cin, pwd);
    } while (pwd.empty());
    auto st = db.createStatement("select id,password from users where name=?");
    st.bindParams(username);
    auto result = st(err);
    int userId = 0;
    if(err == SQLite3::DONE){ // no user with that name was found
        st = db.createStatement("insert into users (name,password) values (?,?)");
        st.bindParams(username, pwd);
        st(err);
        if(err != SQLite3::DONE){
            std::cout << "Uh, something went wrong when signing you up: " << err.what() << '\n';
            return 1;
        }
        std::cout << "Welcome onboard, " << username << "!\n";
        db.execute<decltype(fetchUserId), int&>("select id from users order by id desc limit 1", 
            fetchUserId, userId); // fetch the newly created id
    }
    else if(err == SQLite3::ROW){ // some user was found
        userId = result.at<int32_t>(0);
        auto pass = result.at<std::string_view>(1);
        if(pwd != pass) // wrong password 
        {
            std::cerr << "Uh, incorrect password\n";
            return 1;
        }
        std::cout << "Welcome back " << username << "!\n";
    }
    err = createPost(db, userId);
    if(err != SQLite3::DONE){
        std::cerr << "Oops, something went wrong: " << err.what() << '\n';
        return 1;
    }
    std::cout << "Great! Here are aaaaall the posts of the app:\n";
    err = 
    db.execute("select name,content from users inner join posts on posts.author_id = users.id", printPost);
    if(err){
        std::cout << "Couldn't print the posts: " << err.what() << '\n'; return 1;
    }

    SQLite3::shutdown();
    
    return 0;
}
