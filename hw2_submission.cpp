#include <iostream>
#include <vector>
#include <random>
#include <string>
#include <ctime>
#include <cstdlib>

using namespace std;


class Plane
{
protected:
    double wait_time;
private:
    double pos, vel, distance, loiter_time;
    bool at_SCE = true;
    string origin, destination;
    vector<int> SCE_PHL; // define vectors
    vector<int> SCE_ORD;
    vector<int> SCE_EWR;
    
public:

    Plane() // default constructor
    {
        origin = ""; // define as empty strings
        destination = "";
    } 

    Plane(const string& from, const string& to) // constructor
    {
        origin = from;
        destination = to;
        distance = calc_dist(); // function to calc. distances between various airports

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
            SCE_PHL.push_back(160);
            return SCE_PHL[0];
        }
        else if (origin == "SCE" && destination == "ORD")
        {
            SCE_ORD.push_back(640);
            return SCE_ORD[0];
        }
        else if (origin == "SCE" && destination == "EWR")
        {
            SCE_EWR.push_back(220);
            return SCE_EWR[0];
        }
        else
        {
            return 0.0;
        }
    }
    virtual ~Plane() // virtual deconstuctor
    {
        // empty
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
        return dist(gen); // random # generator between set range
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
    Airliner(const string& airline, const string& from, const string& to) : Plane(from, to) // constructor
        { 
            Airline = airline;
        }


    ~Airliner() {} // empty destructor


    string plane_type() override // override function
    {
        return Airline;
    }


    void time_on_ground() override // override function
    { 
        wait_time = draw_from_normal_dist(1800, 600); // generate random wait time
    }
};

class GeneralAviation : public Plane 
{
    public:
    GeneralAviation(const string& from, const string& to) : Plane(from, to) // constructor
    {
    }
    ~GeneralAviation(){} // empty destructor

    void time_on_ground() override // override function
    {
        wait_time = draw_from_normal_dist(600,60);
    }
};

// Q6

class ATC {
private:
    vector<Plane*> registered_planes;
    const int MAX_LANDED_PLANE_NUM = 2;
    const int AIRSPACE_DISTANCE = 50;

public:
    ATC() // empty constructor
    {
    }

    ~ATC() // empty destructor
    {
    }

    void register_plane(Plane* plane) 
    {
        if (registered_planes.size() < MAX_LANDED_PLANE_NUM) {
            registered_planes.push_back(plane);
        } else 
        {
        }
    }

    void control_traffic()
    {
        int landed_planes = 0;
        int i = 0;

        for (Plane* a_plane : registered_planes) 
        {
            landed_planes += a_plane->getat_SCE();
            i++;

            if (landed_planes >= MAX_LANDED_PLANE_NUM) 
            {
                i = 0; // initialize i

                for (Plane* b_plane : registered_planes) 
                {
                    if (b_plane->getat_SCE() == 0 && b_plane->distance_to_SCE() <= ATC::AIRSPACE_DISTANCE && b_plane->getloiter_time() == 0) 
                    {
                        b_plane->setloiter_time(100);
                        i++;
                    }
                }
            }
        }
    }
};

 int main( int argc, char** argv ) 
{

    // cout << SCE_PHL[0] << endl; // cout statements for initial containers from Q1
    // cout << SCE_ORD[0] << endl;
    // cout << SCE_EWR[0] << endl;

// Q5

    const int num_aircraft = 7;
    Plane* aircraft[num_aircraft]; // create array to hold aircraft

    // instantiate aircraft based on table in Q5
    aircraft[0] = new Airliner("AA", "SCE", "PHL");
    aircraft[1] = new Airliner("UA", "SCE", "ORD");
    aircraft[2] = new Airliner("UA", "SCE", "EWR");
    aircraft[3] = new Airliner("AA", "SCE", "ORD");
    aircraft[4] = new GeneralAviation("SCE", "PHL");
    aircraft[5] = new GeneralAviation("SCE", "EWR");
    aircraft[6] = new GeneralAviation("SCE", "ORD");

    aircraft[0]->setvel(0.130555556);  // miles per second
    aircraft[1]->setvel(0.143055556);
    aircraft[2]->setvel(0.133333333);
    aircraft[3]->setvel(0.138888889);
    aircraft[4]->setvel(0.0388888889);
    aircraft[5]->setvel(0.0444444444);
    aircraft[6]->setvel(0.05);


    const double timeStep = 100; // choose time step
    double currentTime = 0.0;
    double endTime = 1440.0; // 24 hr sim

    ATC atc; // instantiate ATC object

    for (int i = 0; i < num_aircraft; i++)
    {
        atc.register_plane(aircraft[i]);
    }


    while (currentTime < endTime) { // while statement
        for (int i = 0; i < num_aircraft; i++) {
            aircraft[i]->operate(timeStep);
            atc.control_traffic();
            cout << "Type: " << aircraft[i]->plane_type() << " - Position: " << aircraft[i]->getpos() << " miles" << endl;
        }

        currentTime += timeStep;
    }

}

