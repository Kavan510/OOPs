#include<bits/stdc++.h>

using namespace std;

enum class Rank {
    OPERATOR = 0,
    SUPERVISOR = 1,
    DIRECTOR = 2
};

enum class CallState {
    READY = 0,
    IN_PROGRESS = 1,
    COMPLETE = 2
};

class Call;
class CallCenter;

class Employee {
protected:
    int employee_id;
    string name;
    Rank rank;
    Call* call;
    CallCenter* call_center;

public:
    Employee(int id, const string& name, Rank rank, CallCenter* center)
        : employee_id(id), name(name), rank(rank), call(nullptr), call_center(center) {}

    virtual void escalate_call() = 0;

    void take_call(Call* c);
    void complete_call();

protected:
    void _escalate_call();
};

class Call {
public:
    CallState state;
    Rank rank;
    Employee* employee;

    Call(Rank r) : state(CallState::READY), rank(r), employee(nullptr) {}
};

class CallCenter {
public:
    vector<Employee*> operators;
    vector<Employee*> supervisors;
    vector<Employee*> directors;
    deque<Call*> queued_calls;

    CallCenter(const vector<Employee*>& ops, const vector<Employee*>& sups, const vector<Employee*>& dirs)
        : operators(ops), supervisors(sups), directors(dirs) {}

    void dispatch_call(Call* call);
    Employee* _dispatch_call(Call* call, vector<Employee*>& employees);
    void notify_call_escalated(Call* call) {}
    void notify_call_completed(Call* call) {}
    void dispatch_queued_call_to_newly_freed_employee(Call* call, Employee* employee) {}
};

void Employee::take_call(Call* c) {
    call = c;
    call->employee = this;
    call->state = CallState::IN_PROGRESS;
}

void Employee::complete_call() {
    if (call) {
        call->state = CallState::COMPLETE;
        call_center->notify_call_completed(call);
    }
}

void Employee::_escalate_call() {
    if (call) {
        call->state = CallState::READY;
        Call* c = call;
        call = nullptr;
        call_center->notify_call_escalated(c);
    }
}

class Operator : public Employee {
public:
    Operator(int id, const string& name, CallCenter* center)
        : Employee(id, name, Rank::OPERATOR, center) {}

    void escalate_call() override {
        call->rank = Rank::SUPERVISOR;
        _escalate_call();
    }
};

class Supervisor : public Employee {
public:
    Supervisor(int id, const string& name, CallCenter* center)
        : Employee(id, name, Rank::SUPERVISOR, center) {}

    void escalate_call() override {
        call->rank = Rank::DIRECTOR;
        _escalate_call();
    }
};

class Director : public Employee {
public:
    Director(int id, const string& name, CallCenter* center)
        : Employee(id, name, Rank::DIRECTOR, center) {}

    void escalate_call() override {
        throw runtime_error("Directors must handle any call.");
    }
};

void CallCenter::dispatch_call(Call* call) {
    if (call->rank != Rank::OPERATOR && call->rank != Rank::SUPERVISOR && call->rank != Rank::DIRECTOR) {
        throw invalid_argument("Invalid call rank");
    }

    Employee* employee = nullptr;
    if (call->rank == Rank::OPERATOR) {
        employee = _dispatch_call(call, operators);
    }
    if (call->rank == Rank::SUPERVISOR || !employee) {
        employee = _dispatch_call(call, supervisors);
    }
    if (call->rank == Rank::DIRECTOR || !employee) {
        employee = _dispatch_call(call, directors);
    }
    if (!employee) {
        queued_calls.push_back(call);
    }
}

Employee* CallCenter::_dispatch_call(Call* call, vector<Employee*>& employees) {
    for (Employee* employee : employees) {
        if (employee->call == nullptr) {
            employee->take_call(call);
            return employee;
        }
    }
    return nullptr;
}
