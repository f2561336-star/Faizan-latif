#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <fstream>
#include <iomanip>
#include <string>
#include <cstdlib>

using namespace std;

// ==================================================
// PART # 1 - CUSTOM EXCEPTIONS
// ==================================================

class FlightFullException
{
private:
    string message;
public:
    FlightFullException() : message("Flight is Full!") {}
    string what() const { return message; }
};

class DuplicateBookingException
{
private:
    string message;
public:
    DuplicateBookingException() : message("Passenger already booked on this flight!") {}
    string what() const { return message; }
};

class InvalidCancellationException
{
private:
    string message;
public:
    InvalidCancellationException() : message("Invalid Ticket Cancellation!") {}
    string what() const { return message; }
};

// ==================================================
// PART # 2 - FLIGHT ABSTRACT BASE CLASS
// ==================================================

class Flight
{
protected:
    string flightNumber;
    string origin;
    string destination;
    string departureDate;
    int totalSeats;
    int availableSeats;

public:
    Flight(string fn, string org, string dest, string date, int seats)
    {
        flightNumber  = fn;
        origin        = org;
        destination   = dest;
        departureDate = date;
        totalSeats    = seats;
        availableSeats= seats;
    }

    // Pure virtual functions - ABSTRACT CLASS
    virtual double calculateBaseFare() const = 0;
    virtual void   displayDetails()    const = 0;

    // Getters
    string getFlightNumber()   const { return flightNumber; }
    string getOrigin()         const { return origin; }
    string getDestination()    const { return destination; }
    string getDate()           const { return departureDate; }
    int    getAvailableSeats() const { return availableSeats; }
    int    getTotalSeats()     const { return totalSeats; }

    // Seat reservation
    void reserveSeat()
    {
        if (availableSeats <= 0)
        {
            throw FlightFullException();
        }
        availableSeats--;
    }

    void releaseSeat()
    {
        if (availableSeats < totalSeats)
        {
            availableSeats++;
        }
    }

    // Virtual destructor (important for polymorphism)
    virtual ~Flight() {}

    // Operator << overloading
    friend ostream& operator<<(ostream& out, const Flight& flight)
    {
        out << "\nFlight Number : " << flight.flightNumber;
        out << "\nRoute         : " << flight.origin << " -> " << flight.destination;
        out << "\nDeparture     : " << flight.departureDate;
        out << "\nSeats         : " << flight.availableSeats << "/" << flight.totalSeats;
        return out;
    }
};

// ==================================================
// DOMESTIC FLIGHT
// ==================================================

class DomesticFlight : public Flight
{
private:
    double taxRate;

public:
    DomesticFlight(string fn, string org, string dest, string date, int seats, double tax)
        : Flight(fn, org, dest, date, seats)
    {
        taxRate = tax;
    }

    double calculateBaseFare() const
    {
        // Base fare + tax
        return 5000 + (5000 * taxRate);
    }

    void displayDetails() const
    {
        cout << "\n===== DOMESTIC FLIGHT =====";
        cout << *this;
        cout << "\nTax Rate      : " << taxRate * 100 << "%";
        cout << "\nFare          : PKR " << calculateBaseFare() << endl;
    }
};

// ==================================================
// INTERNATIONAL FLIGHT
// ==================================================

class InternationalFlight : public Flight
{
private:
    bool visaRequired;

public:
    InternationalFlight(string fn, string org, string dest, string date, int seats, bool visa)
        : Flight(fn, org, dest, date, seats)
    {
        visaRequired = visa;
    }

    double calculateBaseFare() const
    {
        return 25000;
    }

    void displayDetails() const
    {
        cout << "\n===== INTERNATIONAL FLIGHT =====";
        cout << *this;
        cout << "\nVisa Required : " << (visaRequired ? "Yes" : "No");
        cout << "\nFare          : PKR " << calculateBaseFare() << endl;
    }
};

// ==================================================
// CHARTER FLIGHT
// ==================================================

class CharterFlight : public Flight
{
private:
    string contractHolder;

public:
    CharterFlight(string fn, string org, string dest, string date, int seats, string holder)
        : Flight(fn, org, dest, date, seats)
    {
        contractHolder = holder;
    }

    double calculateBaseFare() const
    {
        return 50000;
    }

    void displayDetails() const
    {
        cout << "\n===== CHARTER FLIGHT =====";
        cout << *this;
        cout << "\nContract Holder: " << contractHolder;
        cout << "\nFare           : PKR " << calculateBaseFare() << endl;
    }
};

// ==================================================
// PART # 3 - PASSENGER ABSTRACT BASE CLASS
// ==================================================

class Passenger
{
protected:
    string passengerID;
    string name;
    string phone;

public:
    Passenger(string id, string pname, string pphone)
    {
        passengerID = id;
        name        = pname;
        phone       = pphone;
    }

    // Pure virtual functions - ABSTRACT CLASS
    virtual double getRefundPercentage() const = 0;
    virtual int    baggageAllowance()    const = 0;
    virtual double loyaltyMultiplier()   const = 0;

    // Getters
    string getPassengerID() const { return passengerID; }
    string getName()        const { return name; }
    string getPhone()       const { return phone; }

    virtual void displayDetails() const
    {
        cout << "\nPassenger ID  : " << passengerID;
        cout << "\nName          : " << name;
        cout << "\nPhone         : " << phone;
    }

    virtual ~Passenger() {}
};

// ==================================================
// ECONOMY PASSENGER
// ==================================================

class EconomyPassenger : public Passenger
{
public:
    EconomyPassenger(string id, string pname, string pphone)
        : Passenger(id, pname, pphone) {}

    double getRefundPercentage() const { return 0.50; }  // 50% refund
    int    baggageAllowance()    const { return 20; }    // 20 KG
    double loyaltyMultiplier()   const { return 1.0; }

    void displayDetails() const
    {
        cout << "\n===== ECONOMY PASSENGER =====";
        Passenger::displayDetails();
        cout << "\nBaggage       : " << baggageAllowance() << " KG";
        cout << "\nRefund Policy : " << getRefundPercentage() * 100 << "%\n";
    }
};

// ==================================================
// BUSINESS PASSENGER
// ==================================================

class BusinessPassenger : public Passenger
{
public:
    BusinessPassenger(string id, string pname, string pphone)
        : Passenger(id, pname, pphone) {}

    double getRefundPercentage() const { return 0.75; }  // 75% refund
    int    baggageAllowance()    const { return 35; }    // 35 KG
    double loyaltyMultiplier()   const { return 2.0; }

    void displayDetails() const
    {
        cout << "\n===== BUSINESS PASSENGER =====";
        Passenger::displayDetails();
        cout << "\nBaggage       : " << baggageAllowance() << " KG";
        cout << "\nRefund Policy : " << getRefundPercentage() * 100 << "%\n";
    }
};

// ==================================================
// FIRST CLASS PASSENGER
// ==================================================

class FirstClassPassenger : public Passenger
{
public:
    FirstClassPassenger(string id, string pname, string pphone)
        : Passenger(id, pname, pphone) {}

    double getRefundPercentage() const { return 0.90; }  // 90% refund
    int    baggageAllowance()    const { return 50; }    // 50 KG
    double loyaltyMultiplier()   const { return 3.0; }

    void displayDetails() const
    {
        cout << "\n===== FIRST CLASS PASSENGER =====";
        Passenger::displayDetails();
        cout << "\nBaggage       : " << baggageAllowance() << " KG";
        cout << "\nRefund Policy : " << getRefundPercentage() * 100 << "%\n";
    }
};

// ==================================================
// PART # 4 - TICKET CLASS (Rule of Three applied)
// ==================================================

class Ticket
{
private:
    string ticketID;
    string flightNumber;
    string passengerID;
    int    seatNumber;
    double farePaid;
    string bookingStatus;

public:
    // Constructor
    Ticket(string tid, string fnum, string pid, int seat, double fare)
    {
        ticketID      = tid;
        flightNumber  = fnum;
        passengerID   = pid;
        seatNumber    = seat;
        farePaid      = fare;
        bookingStatus = "Booked";
    }

    // Getters
    string getTicketID()      const { return ticketID; }
    string getFlightNumber()  const { return flightNumber; }
    string getPassengerID()   const { return passengerID; }
    int    getSeatNumber()    const { return seatNumber; }
    double getFarePaid()      const { return farePaid; }
    string getBookingStatus() const { return bookingStatus; }

    void cancelTicket()
    {
        bookingStatus = "Cancelled";
    }

    // Operator == overloading (compare by ticket ID)
    bool operator==(const Ticket& other) const
    {
        return ticketID == other.ticketID;
    }

    // Operator << overloading
    friend ostream& operator<<(ostream& out, const Ticket& ticket)
    {
        out << "\n==========================";
        out << "\nTicket ID     : " << ticket.ticketID;
        out << "\nPassenger ID  : " << ticket.passengerID;
        out << "\nFlight Number : " << ticket.flightNumber;
        out << "\nSeat Number   : " << ticket.seatNumber;
        out << "\nFare Paid     : PKR " << ticket.farePaid;
        out << "\nStatus        : " << ticket.bookingStatus;
        out << "\n==========================\n";
        return out;
    }
};

// ==================================================
// PART # 5 - AIRLINE CLASS
// ==================================================

class Airline
{
private:
    vector<Flight*>   flights;
    vector<Passenger*>passengers;
    vector<Ticket>    tickets;

public:
    // Constructor
    Airline() {}

    // Destructor - cleanup memory (no memory leaks)
    ~Airline()
    {
        for (int i = 0; i < (int)flights.size(); i++)
        {
            delete flights[i];
        }
        flights.clear();

        for (int i = 0; i < (int)passengers.size(); i++)
        {
            delete passengers[i];
        }
        passengers.clear();
    }

    // =====================================
    // SEARCH FLIGHT BY NUMBER
    // =====================================
    Flight* searchFlightByNumber(const string& flightNo)
    {
        for (int i = 0; i < (int)flights.size(); i++)
        {
            if (flights[i]->getFlightNumber() == flightNo)
            {
                return flights[i];
            }
        }
        return NULL;
    }

    // =====================================
    // SEARCH PASSENGER BY ID
    // =====================================
    Passenger* searchPassengerByID(const string& pID)
    {
        for (int i = 0; i < (int)passengers.size(); i++)
        {
            if (passengers[i]->getPassengerID() == pID)
            {
                return passengers[i];
            }
        }
        return NULL;
    }

    // =====================================
    // ADD FLIGHT
    // =====================================
    void addFlight(Flight* flight)
    {
        if (flight == NULL)
        {
            cout << "\nError: Cannot add NULL flight!\n";
            return;
        }
        flights.push_back(flight);
        cout << "\nFlight " << flight->getFlightNumber() << " Added Successfully!\n";
    }

    // =====================================
    // ADD PASSENGER
    // =====================================
    void addPassenger(Passenger* passenger)
    {
        if (passenger == NULL)
        {
            cout << "\nError: Cannot add NULL passenger!\n";
            return;
        }
        passengers.push_back(passenger);
        cout << "\nPassenger " << passenger->getName() << " Registered Successfully!\n";
    }

    // =====================================
    // LIST ALL FLIGHTS
    // =====================================
    void listFlights()
    {
        if (flights.empty())
        {
            cout << "\nNo Flights Available!\n";
            return;
        }

        cout << "\n========================================\n";
        cout << "           ALL FLIGHTS                  \n";
        cout << "========================================\n";
        for (int i = 0; i < (int)flights.size(); i++)
        {
            flights[i]->displayDetails();  // POLYMORPHISM
            cout << "\n----------------------------------------\n";
        }
    }

    // =====================================
    // LIST ALL PASSENGERS
    // =====================================
    void listPassengers()
    {
        if (passengers.empty())
        {
            cout << "\nNo Passengers Found!\n";
            return;
        }

        cout << "\n========================================\n";
        cout << "          ALL PASSENGERS                \n";
        cout << "========================================\n";
        for (int i = 0; i < (int)passengers.size(); i++)
        {
            passengers[i]->displayDetails();  // POLYMORPHISM
            cout << "\n----------------------------------------\n";
        }
    }

    // =====================================
    // BOOK TICKET
    // BUG FIX: NULL pointer check added
    // =====================================
    void bookTicket(string ticketID, string passengerID, string flightNo)
    {
        // Step 1: Find passenger and flight
        Passenger* passenger = searchPassengerByID(passengerID);
        Flight*    flight    = searchFlightByNumber(flightNo);

        // BUG FIX: check both are found
        if (passenger == NULL)
        {
            cout << "\n[ERROR] Passenger ID '" << passengerID << "' not found!\n";
            return;
        }
        if (flight == NULL)
        {
            cout << "\n[ERROR] Flight '" << flightNo << "' not found!\n";
            return;
        }

        // Step 2: Check for duplicate booking
        for (int i = 0; i < (int)tickets.size(); i++)
        {
            if (tickets[i].getPassengerID() == passengerID &&
                tickets[i].getFlightNumber() == flightNo   &&
                tickets[i].getBookingStatus() == "Booked")
            {
                throw DuplicateBookingException();
            }
        }

        // Step 3: Reserve seat (throws FlightFullException if full)
        flight->reserveSeat();

        // Step 4: Create ticket
        int seatNo = flight->getTotalSeats() - flight->getAvailableSeats();
        Ticket newTicket(ticketID, flightNo, passengerID, seatNo, flight->calculateBaseFare());
        tickets.push_back(newTicket);

        cout << "\n========================================\n";
        cout << "     TICKET BOOKED SUCCESSFULLY!        \n";
        cout << "========================================\n";
        cout << "Ticket ID   : " << ticketID << endl;
        cout << "Passenger   : " << passenger->getName() << endl;
        cout << "Flight      : " << flightNo << endl;
        cout << "Seat Number : " << seatNo << endl;
        cout << "Fare Paid   : PKR " << flight->calculateBaseFare() << endl;
        cout << "Baggage     : " << passenger->baggageAllowance() << " KG\n";
        cout << "========================================\n";
    }

    // =====================================
    // CANCEL TICKET
    // BUG FIX: NULL pointer crash on passenger fixed
    // =====================================
    void cancelTicket(string ticketID)
    {
        for (int i = 0; i < (int)tickets.size(); i++)
        {
            if (tickets[i].getTicketID() == ticketID)
            {
                // Check if already cancelled
                if (tickets[i].getBookingStatus() == "Cancelled")
                {
                    throw InvalidCancellationException();
                }

                Passenger* passenger = searchPassengerByID(tickets[i].getPassengerID());
                Flight*    flight    = searchFlightByNumber(tickets[i].getFlightNumber());

                // BUG FIX: if passenger not found, cannot calculate refund
                if (passenger == NULL)
                {
                    cout << "\n[ERROR] Passenger record not found for this ticket!\n";
                    return;
                }

                // Polymorphic refund calculation
                double refund = tickets[i].getFarePaid() * passenger->getRefundPercentage();
                tickets[i].cancelTicket();

                // Restore seat
                if (flight != NULL)
                {
                    flight->releaseSeat();
                }

                cout << "\n========================================\n";
                cout << "     TICKET CANCELLED SUCCESSFULLY!     \n";
                cout << "========================================\n";
                cout << "Ticket ID     : " << ticketID << endl;
                cout << "Refund Amount : PKR " << fixed << setprecision(2) << refund << endl;
                cout << "========================================\n";
                return;
            }
        }
        // Ticket ID not found at all
        throw InvalidCancellationException();
    }

    // =====================================
    // LIST ALL TICKETS
    // =====================================
    void listTickets()
    {
        if (tickets.empty())
        {
            cout << "\nNo Tickets Found!\n";
            return;
        }

        cout << "\n========================================\n";
        cout << "           ALL TICKETS                  \n";
        cout << "========================================\n";
        for (int i = 0; i < (int)tickets.size(); i++)
        {
            cout << tickets[i];  // operator<< overloading
        }
    }

    // =====================================
    // OCCUPANCY REPORT
    // =====================================
    void occupancyReport()
    {
        cout << "\n========================================\n";
        cout << "         OCCUPANCY REPORT               \n";
        cout << "========================================\n";

        if (flights.empty())
        {
            cout << "No flights available!\n";
            return;
        }

        for (int i = 0; i < (int)flights.size(); i++)
        {
            double occupied   = (double)(flights[i]->getTotalSeats() - flights[i]->getAvailableSeats());
            double percentage = (occupied / flights[i]->getTotalSeats()) * 100.0;

            cout << "\nFlight     : " << flights[i]->getFlightNumber();
            cout << "\nOccupancy  : " << fixed << setprecision(2) << percentage << "%";
            cout << "\nBooked     : " << (int)occupied << "/" << flights[i]->getTotalSeats();
            cout << "\n----------------------------------------\n";
        }
    }

    // =====================================
    // REVENUE REPORT (uses STL map)
    // =====================================
    void revenueReport()
    {
        map<string, double> revenue;

        for (int i = 0; i < (int)tickets.size(); i++)
        {
            if (tickets[i].getBookingStatus() == "Booked")
            {
                revenue[tickets[i].getFlightNumber()] += tickets[i].getFarePaid();
            }
        }

        cout << "\n========================================\n";
        cout << "          REVENUE REPORT                \n";
        cout << "========================================\n";

        if (revenue.empty())
        {
            cout << "No revenue data available!\n";
            return;
        }

        map<string, double>::iterator it;
        for (it = revenue.begin(); it != revenue.end(); it++)
        {
            cout << "\nFlight  : " << it->first;
            cout << "\nRevenue : PKR " << fixed << setprecision(2) << it->second;
            cout << "\n----------------------------------------\n";
        }
    }

    // =====================================
    // SAVE DATA TO FILE
    // =====================================
    void saveData()
    {
        ofstream out("tickets.txt");

        if (!out.is_open())
        {
            cout << "\n[ERROR] Cannot open file for saving!\n";
            return;
        }

        for (int i = 0; i < (int)tickets.size(); i++)
        {
            out << tickets[i].getTicketID()      << ","
                << tickets[i].getPassengerID()   << ","
                << tickets[i].getFlightNumber()  << ","
                << tickets[i].getSeatNumber()    << ","
                << tickets[i].getFarePaid()      << ","
                << tickets[i].getBookingStatus() << endl;
        }

        out.close();
        cout << "\n========================================\n";
        cout << "     DATA SAVED SUCCESSFULLY!           \n";
        cout << "     File: tickets.txt                  \n";
        cout << "========================================\n";
    }

    // =====================================
    // REMOVE FLIGHT
    // =====================================
    void removeFlight(const string& flightNo)
    {
        for (int i = 0; i < (int)flights.size(); i++)
        {
            if (flights[i]->getFlightNumber() == flightNo)
            {
                delete flights[i];
                flights[i] = NULL;
                flights.erase(flights.begin() + i);
                cout << "\nFlight " << flightNo << " Removed Successfully!\n";
                return;
            }
        }
        cout << "\n[ERROR] Flight '" << flightNo << "' Not Found!\n";
    }

    // =====================================
    // REMOVE PASSENGER
    // =====================================
    void removePassenger(const string& pID)
    {
        for (int i = 0; i < (int)passengers.size(); i++)
        {
            if (passengers[i]->getPassengerID() == pID)
            {
                delete passengers[i];
                passengers[i] = NULL;
                passengers.erase(passengers.begin() + i);
                cout << "\nPassenger " << pID << " Removed Successfully!\n";
                return;
            }
        }
        cout << "\n[ERROR] Passenger '" << pID << "' Not Found!\n";
    }
};

// ==================================================
// MAIN FUNCTION
// ==================================================

int main()
{
    Airline airline;
    int    choice;
    string tid, pid, fid;

    system("cls");

    cout << "\n========================================\n";
    cout << "   SKYLINK AIRWAYS MANAGEMENT SYSTEM    \n";
    cout << "========================================\n";
    cout << "     INITIALIZING SYSTEM...             \n";
    cout << "========================================\n\n";

    // ---- Sample Flights (5 flights) ----
    airline.addFlight(new DomesticFlight("PK101", "Islamabad", "Karachi",  "10-June-2026", 100, 0.10));
    airline.addFlight(new DomesticFlight("PK102", "Lahore",    "Karachi",  "15-June-2026",  80, 0.10));
    airline.addFlight(new InternationalFlight("PK201", "Islamabad", "Dubai",  "15-June-2026", 150, true));
    airline.addFlight(new InternationalFlight("PK202", "Karachi",  "London", "20-June-2026", 200, true));
    airline.addFlight(new CharterFlight("CH301", "Islamabad", "Skardu", "05-June-2026", 50, "Travel Experts"));

    // ---- Sample Passengers (5 passengers) ----
    airline.addPassenger(new EconomyPassenger("P001",  "Ali Khan",      "03001234567"));
    airline.addPassenger(new BusinessPassenger("P002", "Ahmed Raza",    "03111234567"));
    airline.addPassenger(new FirstClassPassenger("P003","Sara Ahmed",   "03211234567"));
    airline.addPassenger(new EconomyPassenger("P004",  "Bilal Hussain", "03331234567"));
    airline.addPassenger(new BusinessPassenger("P005", "Fatima Khan",   "03451234567"));

    // ---- Main Menu Loop ----
    do
    {
        cout << "\n\n";
        cout << "========================================\n";
        cout << "          MAIN MENU                     \n";
        cout << "========================================\n";
        cout << "  1.  List All Flights                  \n";
        cout << "  2.  List All Passengers               \n";
        cout << "  3.  Book a Ticket                     \n";
        cout << "  4.  Cancel a Ticket                   \n";
        cout << "  5.  Show All Tickets                  \n";
        cout << "  6.  View Occupancy Report             \n";
        cout << "  7.  View Revenue Report               \n";
        cout << "  8.  Save Data to File                 \n";
        cout << "  9.  Add New Flight                    \n";
        cout << " 10.  Add New Passenger                 \n";
        cout << " 11.  Remove Flight                     \n";
        cout << " 12.  Remove Passenger                  \n";
        cout << "  0.  Exit                              \n";
        cout << "========================================\n";
        cout << "Enter your choice: ";
        cin >> choice;

        try
        {
            if (choice == 1)
            {
                airline.listFlights();
            }
            else if (choice == 2)
            {
                airline.listPassengers();
            }
            else if (choice == 3)
            {
                cout << "\n--- BOOK A TICKET ---\n";
                cout << "Enter Ticket ID    : ";
                cin >> tid;
                cout << "Enter Passenger ID : ";
                cin >> pid;
                cout << "Enter Flight Number: ";
                cin >> fid;
                airline.bookTicket(tid, pid, fid);
            }
            else if (choice == 4)
            {
                cout << "\n--- CANCEL A TICKET ---\n";
                cout << "Enter Ticket ID: ";
                cin >> tid;
                airline.cancelTicket(tid);
            }
            else if (choice == 5)
            {
                airline.listTickets();
            }
            else if (choice == 6)
            {
                airline.occupancyReport();
            }
            else if (choice == 7)
            {
                airline.revenueReport();
            }
            else if (choice == 8)
            {
                airline.saveData();
            }
            else if (choice == 9)
            {
                cout << "\n--- ADD NEW FLIGHT ---\n";
                int    type, seats;
                string fn, org, dest, date, holder;
                double tax;
                bool   visa;
                int    visaInt;

                cout << "Flight Type (1=Domestic, 2=International, 3=Charter): ";
                cin >> type;
                cout << "Flight Number   : ";  cin >> fn;
                cout << "Origin          : ";  cin >> org;
                cout << "Destination     : ";  cin >> dest;
                cout << "Departure Date  : ";  cin >> date;
                cout << "Total Seats     : ";  cin >> seats;

                if (type == 1)
                {
                    cout << "Tax Rate (e.g. 0.10 for 10%): ";
                    cin >> tax;
                    airline.addFlight(new DomesticFlight(fn, org, dest, date, seats, tax));
                }
                else if (type == 2)
                {
                    cout << "Visa Required? (1=Yes, 0=No): ";
                    cin >> visaInt;
                    visa = (visaInt == 1);
                    airline.addFlight(new InternationalFlight(fn, org, dest, date, seats, visa));
                }
                else if (type == 3)
                {
                    cout << "Contract Holder Name: ";
                    cin >> holder;
                    airline.addFlight(new CharterFlight(fn, org, dest, date, seats, holder));
                }
                else
                {
                    cout << "\n[ERROR] Invalid flight type!\n";
                }
            }
            else if (choice == 10)
            {
                cout << "\n--- ADD NEW PASSENGER ---\n";
                int    ptype;
                string id, name, phone;

                cout << "Passenger Type (1=Economy, 2=Business, 3=First Class): ";
                cin >> ptype;
                cout << "Passenger ID : ";  cin >> id;
                cout << "Name         : ";  cin >> name;
                cout << "Phone        : ";  cin >> phone;

                if (ptype == 1)
                    airline.addPassenger(new EconomyPassenger(id, name, phone));
                else if (ptype == 2)
                    airline.addPassenger(new BusinessPassenger(id, name, phone));
                else if (ptype == 3)
                    airline.addPassenger(new FirstClassPassenger(id, name, phone));
                else
                    cout << "\n[ERROR] Invalid passenger type!\n";
            }
            else if (choice == 11)
            {
                cout << "\n--- REMOVE FLIGHT ---\n";
                cout << "Enter Flight Number: ";
                cin >> fid;
                airline.removeFlight(fid);
            }
            else if (choice == 12)
            {
                cout << "\n--- REMOVE PASSENGER ---\n";
                cout << "Enter Passenger ID: ";
                cin >> pid;
                airline.removePassenger(pid);
            }
            else if (choice == 0)
            {
                cout << "\n========================================\n";
                cout << "  Thank you for using SkyLink Airways!  \n";
                cout << "             Goodbye!                   \n";
                cout << "========================================\n";
            }
            else
            {
                cout << "\n[!] Invalid choice! Please enter 0-12.\n";
            }
        }
        catch (FlightFullException& e)
        {
            cout << "\n!!! ERROR: " << e.what() << " !!!\n";
        }
        catch (DuplicateBookingException& e)
        {
            cout << "\n!!! ERROR: " << e.what() << " !!!\n";
        }
        catch (InvalidCancellationException& e)
        {
            cout << "\n!!! ERROR: " << e.what() << " !!!\n";
        }
        catch (...)
        {
            cout << "\n!!! Unknown Error Occurred !!!\n";
        }

    } while (choice != 0);

    return 0;
}

