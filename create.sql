CREATE TABLE user(
    id INT NOT NULL PRIMARY KEY AUTO_INCREMENT,
    name VARCHAR(50),
    age TINYINT UNSIGNED,
    sex ENUM('male', 'female')
);