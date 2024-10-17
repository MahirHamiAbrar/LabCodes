// g++ Assignment_2203077_CSE1204.cpp -o build/Assignment_2203077_CSE1204 && ./build/Assignment_2203077_CSE1204
#include <map>
#include <vector>
#include <time.h>
#include <optional>
#include <iostream>
#include <functional>
using namespace std;

// A custom datatype for storing username and passwords.
typedef struct {
    string name;
    string password;
} UserInfo;

// A custom datatype for storing Vehicle Registration No. & Toll Amount
typedef struct {
    string reg_no;
    int amount;
} VehicleTollInfo;

// A custom datatype for storing Date
typedef struct {
    int day, month, year;
} Date_t;

typedef function<void()> voidfunction_t;    // custom datatype for void lambda functions

// simulates "press enter to exit" functionality
void waitUntilEnterPress() {
    cout << "Press enter to goto main menu...";
    cin.ignore(); cin.clear(); cin.get(); cout << "\n";
}

void getCurrentDateTime(Date_t &current_date, string &time_str) {
    char buf[20];
    struct tm tstruct;
    time_t now = time(0);
    tstruct = *localtime(&now);
    
    strftime(buf, sizeof(buf), "%H:%M", &tstruct);
    time_str = string(buf);

    current_date.day = tstruct.tm_mday;
    current_date.month = tstruct.tm_mon + 1;
    current_date.year = tstruct.tm_year + 1900;
}

// The Vehicle Class. (inherited by the Bus, Truck & Car classes)
class Vehicle {
    protected:
        Date_t Date;
        string __vehicle_name;
        string Reg_No, Time, Operator_ID;
        VehicleTollInfo takeVehicleToll_(const string &__vehicle_name, const int __toll_price);
        void collectTollFromOperator_(int __original_amount, int __given_amount);
    
    public:
        Vehicle();  // Default Constructor
        
        // pure virtual functions (must be overridden)
        virtual int getTollPrice() = 0;
        virtual int64_t tollCalculate() = 0;
        virtual void getInput(const string &__operator_id) = 0;

        // data member getter methods
        string getVehicleName() const { return __vehicle_name; }
        string getRegNo() const { return this->Reg_No; }
        string getDateString() const { return (to_string(Date.day) + "/" + to_string(Date.month) + "/" + to_string(Date.year)); }
        string getTime() const { return this->Time; }
        string getOperator() const { return this->Operator_ID; }
        Date_t getDate() const { return Date; }
};


class Bus : public Vehicle {
    private:
        int Seats;
        static int tollPrice;
        static int vehicleCount;
    
    public:
        Bus();  // Default Constructor
        int getTollPrice() { return this->tollPrice; };
        static void setTollPrice(int __price) { tollPrice = __price; }
        static int getVehicleCount() { return vehicleCount; }

        void getInput(const string &__operator_id) override {
            VehicleTollInfo tollInfo = takeVehicleToll_(getVehicleName(), this->tollPrice);
            collectTollFromOperator_(this->tollPrice, tollInfo.amount);

            this->Reg_No = tollInfo.reg_no;
            this->Operator_ID = __operator_id;
            getCurrentDateTime(this->Date, this->Time);

            ++vehicleCount;   // increase bus count
        }

        int64_t tollCalculate() override {
            return (int64_t)this->vehicleCount * (int64_t)this->tollPrice;
        }
};


class Truck : public Vehicle {
    private:
        int Weight;
        int Height;
        static int tollPrice;
        static int vehicleCount;
    
    public:
        Truck();    // Default Constructor
        int getTollPrice() { return this->tollPrice; };
        static void setTollPrice(int __price) { tollPrice = __price; }
        static int getVehicleCount() { return vehicleCount; }

        void getInput(const string &__operator_id) override {
            VehicleTollInfo tollInfo = takeVehicleToll_(getVehicleName(), this->tollPrice);
            collectTollFromOperator_(this->tollPrice, tollInfo.amount);

            this->Reg_No = tollInfo.reg_no;
            this->Operator_ID = __operator_id;
            getCurrentDateTime(this->Date, this->Time);

            ++vehicleCount;   // increase bus count
        }

        int64_t tollCalculate() override {
            return (int64_t)this->vehicleCount * (int64_t)this->tollPrice;
        }
};


class Car : public Vehicle {
    private:
        string Owner;
        static int tollPrice;
        static int vehicleCount;
    
    public:
        Car();  // Default Constructor
        int getTollPrice() { return this->tollPrice; };
        static void setTollPrice(int __price) { tollPrice = __price; }
        static int getVehicleCount() { return vehicleCount; }

        string getOwner() const { return this->Owner; }
        void setOwner(const string &__owner_name) { this->Owner = __owner_name; }

        void getInput(const string &__operator_id) override {
            VehicleTollInfo tollInfo = takeVehicleToll_(getVehicleName(), this->tollPrice);
            collectTollFromOperator_(this->tollPrice, tollInfo.amount);

            this->Reg_No = tollInfo.reg_no;
            this->Operator_ID = __operator_id;
            getCurrentDateTime(this->Date, this->Time);

            ++vehicleCount;   // increase bus count
        }

        int64_t tollCalculate() override {
            return (int64_t)this->vehicleCount * (int64_t)this->tollPrice;
        }
};

class TollPlaza {

    private:
        const int title_max_length_ = 40;
        string current_logged_in_username_ = "";
        bool __is_running = true, __search_menu_open = false, __tp_menu_open = false;

        vector<UserInfo> users_;
        
        const vector<const char*> lr_menu_items = {"Login", "Register", "Exit"};
        const vector<const char*> tp_menu_items = {"Bus", "Truck", "Car", "Search", "Statistics", "Toll Settings (admin)", "Save and Logout"};
        const vector<const char*> search_menu_items = {"Vehicle", "Date to Date", "Operator", "Back"};

        // login-registration, toll-plaza & search menu functions,
        map<int, voidfunction_t> lr_menu_functions, tp_menu_functions, search_menu_functions;

        optional<UserInfo> searchUser_(const string &__name);
        int printMenuItems_(const string &title, const vector<const char*> &items);
        int getOption_(const int maxOptionNum);
        void collectToll_(Vehicle *__vehicle);
        void printFormattedRow_(const int __cell_width, const vector<string> &__cells);
        bool dateIsWithinRange_(const Date_t &__start_date, const Date_t &__end_date, const Date_t &__cur_date);
    
    public:
        vector<Vehicle *> vehicles_;
        TollPlaza();    // Default Constructor

        bool isRunning() const {
            return this->__is_running;
        }

        /* Menu Displaying Methods */
        void showLoginRegistrationMenu();
        void showTollPlazaMenu();
        void showSearchMenu();

        /* Login-Registration Menu Functions */
        void loginUser();
        void registerUser();
        void exitProgram();

        /* Toll-Plaza Menu Functions */
        void takeBusToll();
        void takeTruckToll();
        void takeCarToll();
        void showStatistics();
        void adminTollSettings();
        void saveAndLogoutTollPlaza();

        /* Search Menu Functions */
        void vehicleSearch();
        void dateToDateSearch();
        void operatorSearch();
        void backFromSearch();
};


/* Set Default Values to the Static Data Members */
int Bus::tollPrice = 500;
int Car::tollPrice = 150;
int Truck::tollPrice = 400;

int Bus::vehicleCount = 0;
int Car::vehicleCount = 0;
int Truck::vehicleCount = 0;


int main() {
    TollPlaza tp;
    while (tp.isRunning()) tp.showLoginRegistrationMenu();
    return 0;
}


/* Vehicle Class Implementation */
Vehicle::Vehicle() {}

VehicleTollInfo Vehicle::takeVehicleToll_(const string &__vehicle_name, const int __toll_price) {
    VehicleTollInfo vti;
    cout << "Enter " << __vehicle_name << " Registration No: ";
    cin >> vti.reg_no;

    printf("Enter Amount (Tk. %d): ", __toll_price);
    cin >> vti.amount;

    return vti;
}

void Vehicle::collectTollFromOperator_(int __original_amount, int __given_amount) {
    int return_amount = __given_amount - __original_amount;
    if (return_amount > 0)
        cout << "Return Amount: " << return_amount << "\n";
}


/* Bus Class Implementation */
Bus::Bus() { __vehicle_name = "Bus"; }

/* Truck Class Implementation */
Truck::Truck() { __vehicle_name = "Truck"; }

/* Car Class Implementation */
Car::Car() { __vehicle_name = "Car"; }


/* Toll Plaza Class Implementation */
TollPlaza::TollPlaza(){
    // register login-registration functions (using lambda-functions)
    lr_menu_functions[1] = [this]() { loginUser(); };
    lr_menu_functions[2] = [this]() { registerUser(); };
    lr_menu_functions[3] = [this]() { exitProgram(); };

    // register toll-plaza menu functions
    tp_menu_functions[1] = [this]() { takeBusToll(); };
    tp_menu_functions[2] = [this]() { takeTruckToll(); };
    tp_menu_functions[3] = [this]() { takeCarToll(); };
    tp_menu_functions[4] = [this]() { showSearchMenu(); };
    tp_menu_functions[5] = [this]() { showStatistics(); };
    tp_menu_functions[6] = [this]() { adminTollSettings(); };
    tp_menu_functions[7] = [this]() { saveAndLogoutTollPlaza(); };

    // register search menu functions
    search_menu_functions[1] = [this]() { vehicleSearch(); };
    search_menu_functions[2] = [this]() { dateToDateSearch(); };
    search_menu_functions[3] = [this]() { operatorSearch(); };
    search_menu_functions[4] = [this]() { backFromSearch(); };
}

optional<UserInfo> TollPlaza::searchUser_(const string &__name) {
    for (auto user_: users_)
        if (user_.name.compare(__name) == 0) return user_;
    return nullopt;
}

int TollPlaza::printMenuItems_(const string &title, const vector<const char*> &items) {
    int star_count = (title_max_length_ - title.size()) / 2 - 2;
    string stars = ""; while(star_count--) stars += "*";
    cout.width(title_max_length_); cout << "\n" + stars + " " + title + " " + stars + "\n";

    for (int i = 0; i < items.size(); ++i)
        cout << "    " << (i + 1) << ". " << items[i] << '\n';
    return (int)items.size();   // returns the value of 'maxOptionNum'
}

int TollPlaza::getOption_(const int maxOptionNum) {
    int op;
    while(__is_running) {
        printf("Enter Your Option (1-%d): ", maxOptionNum);
        cin >> op;
        if (op >= 1 && op <= maxOptionNum) break;
        cout << "Invalid Option.\n";
    }
    return op;
}

void TollPlaza::collectToll_(Vehicle *__vehicle) {
    __vehicle->getInput(current_logged_in_username_);
    vehicles_.push_back(__vehicle);
    cout << "Toll Collection is Successful.\n"; waitUntilEnterPress();
}

void TollPlaza::printFormattedRow_(const int __cell_width, const vector<string> &__cells) {
    cout.clear();
    for (string __cell: __cells) {
        cout.width(__cell_width); cout << left << __cell;
    }
    cout << "\n";
}

bool TollPlaza::dateIsWithinRange_(const Date_t &__start_date, const Date_t &__end_date, const Date_t &__cur_date) {
    int __sd_num = __start_date.year * 10000 + __start_date.month * 100 + __start_date.day;
    int __ed_num = __end_date.year * 10000 + __end_date.month * 100 + __end_date.day;
    int __cd_num = __cur_date.year * 10000 + __cur_date.month * 100 + __cur_date.day;
    return (__cd_num >= __sd_num && __cd_num <= __ed_num);
}

void TollPlaza::showLoginRegistrationMenu() {
    int maxOptionNum = printMenuItems_("Toll Plaza: Operator", lr_menu_items);
    int option = getOption_(maxOptionNum);  cout << "\n";
    lr_menu_functions[option]();    // call the function orresponding to the user-selected option
}

void TollPlaza::showTollPlazaMenu() {
    __tp_menu_open = true;
    
    while (__tp_menu_open) {
        int maxOptionNum = printMenuItems_("Toll Plaza Menu", tp_menu_items);
        int option = getOption_(maxOptionNum);  cout << "\n";
        // call the function orresponding to the user-selected option
        tp_menu_functions[option]();
    }
}

void TollPlaza::showSearchMenu() {
    __search_menu_open = true;

    while (__search_menu_open) {
        int maxOptionNum = printMenuItems_("Toll Plaza Search", search_menu_items);
        int option = getOption_(maxOptionNum);  cout << "\n";
        // call the function orresponding to the user-selected option
        search_menu_functions[option]();
    }
}

void TollPlaza::loginUser() {
    UserInfo user;
    cout << "Enter Username: "; getline(cin >> ws, user.name);
    cout << "Enter Password: "; getline(cin >> ws, user.password);

    auto search_result = searchUser_(user.name);
    if (search_result == nullopt) {
        cout << "Invalid Username.\n";
        return;
    }

    UserInfo existing_user = *search_result;
    if (existing_user.password != user.password) {
        cout << "Invalid Password.\n";
        return;
    }

    // If the program comes this far, then the authentication is successful. Show the toll plaza manu now.
    current_logged_in_username_ = user.name;
    showTollPlazaMenu();
}

void TollPlaza::registerUser() {
    UserInfo user;
    cout << "Enter Username: "; getline(cin >> ws, user.name);

    auto search_result = searchUser_(user.name);
    if (search_result != nullopt) {
        cout << "Operator already exists.\n";
        return;
    }

    string confirm_pass;
    cout << "Enter Password: "; getline(cin >> ws, user.password);
    cout << "Reenter the Password: "; getline(cin >> ws, confirm_pass);

    if (confirm_pass != user.password) {
        cout << "Passwords Does Not Match. Please Retry.\n";
        return;
    }

    users_.push_back(user);
    cout << "Registration is Successful.\n"; waitUntilEnterPress();
}

void TollPlaza::exitProgram() {
    __is_running = false;
}

void TollPlaza::takeBusToll() {
    Bus *bus = new Bus(); collectToll_(bus);
}

void TollPlaza::takeTruckToll() {
    Truck *truck = new Truck(); collectToll_(truck);
}

void TollPlaza::takeCarToll() {
    Car *car = new Car(); collectToll_(car);
}

void TollPlaza::showStatistics() {
    const int __cell_width = 10;
    Bus _b; Truck _t; Car _c;
    int64_t busToll = _b.tollCalculate(), truckToll = _t.tollCalculate(), carToll = _c.tollCalculate();

    printFormattedRow_(__cell_width, {"Vehicle", "Number", "Amount"});
    printFormattedRow_(__cell_width, {"Bus", to_string(_b.getVehicleCount()), to_string(busToll)});
    printFormattedRow_(__cell_width, {"Truck", to_string(_t.getVehicleCount()), to_string(truckToll)});
    printFormattedRow_(__cell_width, {"Car", to_string(_c.getVehicleCount()), to_string(carToll)});

    cout << "Total Amount: " << (busToll + truckToll + carToll) << '\n';
    waitUntilEnterPress();
}

void TollPlaza::adminTollSettings() {
    // Admin Login System
    string __uname, __pass;
    cout << "Enter Admin Username: "; cin >> __uname;
    cout << "Enter Admin Password: "; cin >> __pass;

    if (__uname != "admin") { cout << "Invalid Username.\n"; return; }
    if (__pass != "admin") { cout << "Invalid Password.\n"; return; }

    // Successful login! Now setup tolls
    int __toll;
    cout << "Enter Toll for Bus: ";     cin >> __toll;  Bus::setTollPrice(__toll);
    cout << "Enter Toll for Track: ";   cin >> __toll;  Truck::setTollPrice(__toll);
    cout << "Enter Toll for Car: ";     cin >> __toll;  Car::setTollPrice(__toll);

    cout << "Toll Change is Successful.\n"; waitUntilEnterPress();
}

void TollPlaza::saveAndLogoutTollPlaza() {
    __tp_menu_open = false;
    current_logged_in_username_ = "";
    cout << "Saved All Changes & Logged Out!\n"; waitUntilEnterPress();
}

void TollPlaza::vehicleSearch() {
    cout << "Enter Vehicle Registration No: ";
    string __reg_no; cin >> __reg_no;

    vector<vector<string>> __rows = {{"Date", "Time", "Amount", "Operator"}};

    for (auto &v: this->vehicles_)
        if (v->getRegNo() == __reg_no) {
            __rows.push_back({
                v->getDateString(),
                v->getTime(),
                to_string(v->getTollPrice()),
                v->getOperator()
            });
        }

    if (__rows.size() == 1) { cout << "Vehicle NOT Found.\n"; return; }
    for (vector<string> &__row: __rows) printFormattedRow_(15, __row);
    waitUntilEnterPress();
}

void TollPlaza::dateToDateSearch() {
    Date_t start_date, end_date;
    cout << "Enter Start Date (DD MM YYYY): ";
    cin >> start_date.day >> start_date.month >> start_date.year;
    cout << "Enter End Date (DD MM YYYY): ";
    cin >> end_date.day >> end_date.month >> end_date.year;

    vector<vector<string>> __rows = {{"Date", "Time", "Vehicle", "Amount", "Operator"}};

    for (const auto &v: vehicles_)
        if (dateIsWithinRange_(start_date, end_date, v->getDate())) {
            __rows.push_back({
                v->getDateString(),
                v->getTime(),
                v->getVehicleName(),
                to_string(v->getTollPrice()),
                v->getOperator()
            });
        }

    if (__rows.size() == 1) { cout << "Vehicle NOT Found.\n"; return; }
    for (vector<string> &__row: __rows) printFormattedRow_(15, __row);
    waitUntilEnterPress();
    
}

void TollPlaza::operatorSearch() {
    cout << "Enter Operator Name: ";
    string name; cin >> name;

    vector<vector<string>> __rows = {{"Operator", "Vehicle", "Date", "Time", "Amount"}};

    for (const auto &v: vehicles_)
        if (v->getOperator() == name) {
            __rows.push_back({
                v->getOperator(),
                v->getVehicleName(),
                v->getDateString(),
                v->getTime(),
                to_string(v->getTollPrice())
            });
        }

    if (__rows.size() == 1) { cout << "Operator NOT exists.\n"; return; }
    for (vector<string> &__row: __rows) printFormattedRow_(15, __row);
    waitUntilEnterPress();
}

void TollPlaza::backFromSearch() {
    __search_menu_open = false;
}
