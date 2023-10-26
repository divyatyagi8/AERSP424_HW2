#include <iostream>
#include "q1.h"
#include <random>

using namespace std;

class Plane 
{
    protected:
        double wait_time = 0;
    private:
        double pos, vel, distance, loiter_time;
        bool at_SCE = true;   
        string origin, destination;
        vector<int> SCE_PHL;
        vector<int> SCE_ORD;
        vector<int> SCE_EWR;
    public:
        string from, to; // inputs for constructor
        Plane(string from, string to) // constructor
        {
            string origin = from;
            string destination = to;
        }
        virtual ~Plane()
        {

        }
        void operate (double dt)
        {
            
        }
        double getpos()
        {
            pos = 0;
            return pos;
        }
        double getvel()
        {
            vel = 0;
            return vel;
        }
        double getloiter_time()
        {
            loiter_time = 0;
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
            at_SCE = 0;
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
            double dist_PHL = SCE_PHL[0];
            double dist_ORD = SCE_ORD[0];
            double dist_EWR = SCE_EWR[0];
        }
        double virtual time_on_ground()
        {

        }
        string virtual plane_type()
        {

        }
        static int draw_from_normal_dist(double mean, double stdev)
        {
            double max = mean + stdev;
            double min = mean - stdev;
            random_device rd;
            mt19937 gen(rd());
            uniform_int_distribution<int> dist(min, max); // range
            return dist(gen);
        }

        
};  

int main()
{



}