create database server;
use server;
CREATE TABLE Users (
  user_id INT PRIMARY KEY AUTO_INCREMENT,
  user_name VARCHAR(50),
  user_pwd VARCHAR(50)
);

CREATE TABLE Files (
  file_id INT PRIMARY KEY AUTO_INCREMENT, 
  file_name VARCHAR(100),
  file_path VARCHAR(1000),
  user_id INT,
  FOREIGN KEY (user_id)
    REFERENCES Users(user_id)
);