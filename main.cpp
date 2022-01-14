#include <iostream>
#include <thread>
#include <mutex>
#include <algorithm>
#include <map>
#include <vector>
#include <algorithm>

std::mutex pace_record_access;

struct swimmer
{
    swimmer(std::string name, double pace)
    {
        this->name=name;
        this->pace=pace;
    }

    swimmer(){}

    std::string name;
    double pace;
};

bool race_result_compare(swimmer a, swimmer b)
{
    return a.pace<b.pace;
}

std::vector<swimmer> race_results;

void swim(std::string name, int pace)
{
    int time=1;
    while(pace*time<100)
    {
        std:: cout << "Swimmer " << name << " swam " << pace*time << " meters\n";
        std::this_thread::sleep_for(std::chrono::seconds(1));
        time++;
    }

    std:: cout << "Swimmer " << name << " finished with " << 100.0/pace << " seconds\n";
    pace_record_access.lock();
    race_results.push_back(swimmer (name, 100.0/pace));
    pace_record_access.unlock();
}

int main()
{
    swimmer athletes[6];

    for(int i=0;i<6;i++)
    {
        std::cout << "Enter " << i+1 << " swimmer name: ";
        std::cin >> athletes[i].name;
        std::cout << "And his pace: ";
        std::cin >> athletes[i].pace;
    }

    for(int i=0;i<6;i++)
    {
        std::thread (swim, athletes[i].name, athletes[i].pace).detach();
    }

    while(race_results.size()!=6)
    {
        continue;
    }

    std::sort(race_results.begin(), race_results.end(), race_result_compare);

    for(auto i=race_results.begin();i!=race_results.end();i++)
    {
        std::cout << i->name << " " << i->pace<< std::endl;
    }

    return 0;
}