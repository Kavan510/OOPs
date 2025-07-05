#include <iostream>
#include <string>
#include <vector>
#include <deque>
#include <stdexcept>

using namespace std;

// Enums for employee rank and call state
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

// Forward declarations
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
    void notify_call_escalated(Call* call);
    void notify_call_completed(Call* call);
    void dispatch_queued_call_to_newly_freed_employee(Employee* employee);
};

void Employee::take_call(Call* c) {
    call = c;
    call->employee = this;
    call->state = CallState::IN_PROGRESS;
    cout << "Call taken by employee ID " << employee_id << endl;
}

void Employee::complete_call() {
    if (call) {
        call->state = CallState::COMPLETE;
        cout << "Call completed by employee ID " << employee_id << endl;
        call_center->notify_call_completed(call);
        call = nullptr;
        call_center->dispatch_queued_call_to_newly_freed_employee(this);
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
        if (!call) return;
        cout << "Call escalated by Operator ID " << employee_id << endl;
        call->rank = Rank::SUPERVISOR;
        _escalate_call();
    }
};

class Supervisor : public Employee {
public:
    Supervisor(int id, const string& name, CallCenter* center)
        : Employee(id, name, Rank::SUPERVISOR, center) {}

    void escalate_call() override {
        if (!call) return;
        cout << "Call escalated by Supervisor ID " << employee_id << endl;
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
    Employee* employee = nullptr;

    if (call->rank == Rank::OPERATOR) {
        employee = _dispatch_call(call, operators);
    }

    if (!employee && call->rank <= Rank::SUPERVISOR) {
        employee = _dispatch_call(call, supervisors);
    }

    if (!employee && call->rank <= Rank::DIRECTOR) {
        employee = _dispatch_call(call, directors);
    }

    if (!employee) {
        cout << "No employee available. Call queued.\n";
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

void CallCenter::notify_call_escalated(Call* call) {
    cout << "Call escalated to higher rank.\n";
    dispatch_call(call);
}

void CallCenter::notify_call_completed(Call* call) {
    cout << "Call completed and resources freed.\n";
}

void CallCenter::dispatch_queued_call_to_newly_freed_employee(Employee* employee) {
    if (!queued_calls.empty()) {
        for (auto it = queued_calls.begin(); it != queued_calls.end(); ++it) {
            if (static_cast<int>((*it)->rank) <= static_cast<int>(employee->rank)) {
                Call* call = *it;
                queued_calls.erase(it);
                employee->take_call(call);
                cout << "Queued call assigned to employee ID " << employee->employee_id << endl;
                return;
            }
        }
    }
}
