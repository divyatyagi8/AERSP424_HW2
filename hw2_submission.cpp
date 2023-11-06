#include <iostream>
#include <vector>
#include <random>
#include <string>
#include <ctime>
#include <cstdlib>

using namespace std;

vector<int> SCE_PHL; // define vectors
vector<int> SCE_ORD;
vector<int> SCE_EWR;

void dist_PHL()
{
    SCE_PHL.push_back(160);
}

void dist_ORD()
{
    SCE_ORD.push_back(640);
}

void dist_EWR()
{
    SCE_EWR.push_back(220);
}


class Plane
{
protected:
    double wait_time;
private:
    double pos, vel, distance, loiter_time;
    bool at_SCE = true;
    string origin, destination;
    vector<int> SCE_PHL;
    vector<int> SCE_ORD;
    vector<int> SCE_EWR;
    double dist_PHL = SCE_PHL[0];
    double dist_ORD = SCE_ORD[0];
    double dist_EWR = SCE_EWR[0];
public:

    Plane() // default constructor
    {
        origin = "";
        destination = "";
        distance = 0.0;
        pos = 0.0;
        vel = 0.0;
        wait_time = 0.0;
        loiter_time = 0.0;
        at_SCE = false;
    }
    Plane(string& from, string& to) // constructor
    {
        origin = from;
        destination = to;
        distance = calc_dist();

            pos = 0.0;
            vel = 0.0;
            wait_time = 0.0;
            loiter_time = 0.0;
            at_SCE = false;
    }

    double calc_dist()
    {
        if (origin == "SCE" && destination == "PHL")
        {
            return SCE_PHL[0];
        }
        else if (origin == "SCE" && destination == "ORD")
        {
            return SCE_ORD[0];
        }
        else if (origin == "SCE" && destination == "EWR")
        {
            return SCE_EWR[0];
        }
        else
        {
            return 0.0;
        }
    }
    virtual ~Plane() // virtual deconstuctor
    {

    }
    double getpos()
    {
        return pos;
    }
    double getvel()
    {
        return vel;
    }
    double getloiter_time()
    {
        return loiter_time;
    }
    string getorigin()
    {
        return origin;
    }
    string getdestination()
    {
        return destination;
    }
    bool getat_SCE()
    {
        return at_SCE;
    }
    void setvel(double velinput)
    {
        vel = velinput;
    }
    void setloiter_time(double ltinput)
    {
        loiter_time = ltinput;
    }
    double distance_to_SCE()
    {
        if (destination == "SCE")
        {
            return distance - pos;
        }
    }
    void virtual time_on_ground()
    {
        swap(origin, destination);
        pos = 0.0;
    }
    string virtual plane_type()
    {
        string GA = "GA";
        return GA;
    }
    static double draw_from_normal_dist(double mean, double stdev)
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::normal_distribution<double> dist(mean, stdev);
        return dist(gen);
    }

    void operate(double dt)
    {
        if (loiter_time != 0)
        {
            if (loiter_time < 0)
            {
                loiter_time = 0;
            }
            loiter_time -= dt;
        }
        else
        {
            if (wait_time != 0)
            {
                if (wait_time < 0)
                {
                    wait_time = 0;
                }
                wait_time -= dt;
            }
            else
            {
                if (pos < distance)
                {
                    pos += vel * dt;
                    at_SCE = 0;
                }
                else
                {
                    if (destination == "SCE")
                    {
                        at_SCE = 1;
                    }
                    else
                    {
                        time_on_ground();
                    }
                }
            }
        }
    }

};


class Airliner : public Plane {
private:
    string Airline;

public:
    Airliner(const string& airline, const string& from, const string& to)
        { 
            Airline = airline;
        }

    // Destructor with an empty implementation
    ~Airliner() {}

    // Override plane_type to return the airline
    string plane_type() override {
        return Airline;
    }

    // Override time_on_ground to draw wait_time with mean 1800 and standard deviation 600
    void time_on_ground() override {
        wait_time = draw_from_normal_dist(1800, 600);
    }
};

class GeneralAviation : public Plane 
{
    public:
    GeneralAviation(const string& from, const string& to)
    {
    }
    ~GeneralAviation(){}

    void time_on_ground() override
    {
        wait_time = draw_from_normal_dist(600,60);
    }
};

// Q6

class ATC {
private:
    vector<Plane*> registered_planes;
    const int MAX_LANDED_PLANE_NUM;
    const int AIRSPACE_DISTANCE;

public:
    ATC() : MAX_LANDED_PLANE_NUM(2), AIRSPACE_DISTANCE(50) {
        // Constructor sets the values for MAX_LANDED_PLANE_NUM and AIRSPACE_DISTANCE
    }

    ~ATC() {
        // Destructor (empty implementation)
    }

    void register_plane(Plane* plane) {
        if (registered_planes.size() < MAX_LANDED_PLANE_NUM) {
            // Check if the maximum number of planes allowed to land has not been reached
            registered_planes.push_back(plane);
            cout << "Plane registered with ATC." << endl;
        } else {
            cout << "ATC: Maximum number of planes allowed to land has been reached." << endl;
        }
    }
};



int main()
{
    cout << SCE_PHL[0] << endl;
    cout << SCE_ORD[0] << endl;
    cout << SCE_EWR[0] << endl;

// Q5
    // Create an array of pointers to the base class
    const int num_aircraft = 7;
    Plane* aircraft[num_aircraft];

    // Instantiate aircraft based on your specifications
    aircraft[0] = new Airliner("AA", "SCE", "PHL");
    aircraft[1] = new Airliner("UA", "SCE", "ORD");
    aircraft[2] = new Airliner("UA", "SCE", "EWR");
    aircraft[3] = new Airliner("AA", "SCE", "ORD");
    aircraft[4] = new GeneralAviation("SCE", "PHL");
    aircraft[5] = new GeneralAviation("SCE", "EWR");
    aircraft[6] = new GeneralAviation("SCE", "ORD");

    aircraft[0]->setvel(470);  // mph
    aircraft[1]->setvel(515);
    aircraft[2]->setvel(480);
    aircraft[3]->setvel(500);
    aircraft[4]->setvel(140);
    aircraft[5]->setvel(160);
    aircraft[6]->setvel(180);

    // Simulation parameters
    const double minTimeStep = 10.0;
    const double maxTimeStep = 100.0;

    double currentTime = 0.0;
    double endTime = 1440.0; // Simulation runs for 24 hours

    // print out pos of airplane at each time step
    while (currentTime < endTime) {
        double timeStep = minTimeStep + (rand() / (double)RAND_MAX) * (maxTimeStep - minTimeStep);

        for (int i = 0; i < num_aircraft; i++) {
            aircraft[i]->operate(timeStep);
            cout << "Type: " << aircraft[i]->plane_type() << " - Position: " << aircraft[i]->getpos() << " miles" << endl;
        }

        currentTime += timeStep;
    }

}

