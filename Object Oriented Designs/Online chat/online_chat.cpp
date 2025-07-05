#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <string>
#include <memory>
#include <ctime>

using namespace std;

// Forward declarations
class User;
class PrivateChat;
class GroupChat;
class AddRequest;

// Enum for Request Status
enum class RequestStatus {
    UNREAD,
    READ,
    ACCEPTED,
    REJECTED
};

// Message Class
class Message {
public:
    int message_id;
    string message;
    time_t timestamp;

    Message(int id, const string& msg, time_t ts)
        : message_id(id), message(msg), timestamp(ts) {}
};

// Abstract Chat class
class Chat {
public:
    int chat_id;
    vector<shared_ptr<User>> users;
    vector<Message> messages;

    Chat(int id) : chat_id(id) {}
    virtual ~Chat() = default;
};

// Private Chat
class PrivateChat : public Chat {
public:
    PrivateChat(int id, shared_ptr<User> user1, shared_ptr<User> user2)
        : Chat(id) {
        users.push_back(user1);
        users.push_back(user2);
    }
};

// Group Chat
class GroupChat : public Chat {
public:
    GroupChat(int id) : Chat(id) {}

    void add_user(shared_ptr<User> user) {
        users.push_back(user);
    }

    void remove_user(shared_ptr<User> user) {
        users.erase(remove(users.begin(), users.end(), user), users.end());
    }
};

// AddRequest class
class AddRequest {
public:
    int from_user_id;
    int to_user_id;
    RequestStatus status;
    time_t timestamp;

    AddRequest(int from_id, int to_id, RequestStatus stat, time_t ts)
        : from_user_id(from_id), to_user_id(to_id), status(stat), timestamp(ts) {}
};

// User class
class User {
public:
    int user_id;
    string name;
    string pass_hash;
    unordered_map<int, shared_ptr<User>> friends_by_id;
    unordered_map<int, shared_ptr<PrivateChat>> private_chats;
    unordered_map<int, shared_ptr<GroupChat>> group_chats;
    unordered_map<int, shared_ptr<AddRequest>> received_requests;
    unordered_map<int, shared_ptr<AddRequest>> sent_requests;

    User(int id, const string& nm, const string& pass)
        : user_id(id), name(nm), pass_hash(pass) {}

    void send_message_to_user(int friend_id, const string& message) {}
    void send_message_to_group(int group_id, const string& message) {}
    void send_friend_request(int friend_id) {}
    void receive_friend_request(int friend_id) {}
    void approve_friend_request(int friend_id) {}
    void reject_friend_request(int friend_id) {}
};

// UserService class
class UserService {
public:
    unordered_map<int, shared_ptr<User>> users_by_id;

    void add_user(int user_id, const string& name, const string& pass_hash) {}
    void remove_user(int user_id) {}
    void add_friend_request(int from_user_id, int to_user_id) {}
    void approve_friend_request(int from_user_id, int to_user_id) {}
    void reject_friend_request(int from_user_id, int to_user_id) {}
};
