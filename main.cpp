#include <iostream>
#include <thread>
#include <mutex>
#include <algorithm>
#include <map>
#include <vector>
#include <algorithm>

std::mutex pace_record_access;
std::mutex coutMutex;

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
        coutMutex.lock();
        std:: cout << "Swimmer " << name << " swam " << pace*time << " meters\n";
        coutMutex.unlock();
        std::this_thread::sleep_for(std::chrono::seconds(1));
        time++;
    }

    coutMutex.lock();
    std:: cout << "Swimmer " << name << " finished with " << 100.0/pace << " seconds\n";
    coutMutex.unlock();
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

    std::thread swimmer[6];

    std::thread swimmer1(swim, athletes[0].name, athletes[0].pace);
    std::thread swimmer2(swim, athletes[1].name, athletes[1].pace);
    std::thread swimmer3(swim, athletes[2].name, athletes[2].pace);
    std::thread swimmer4(swim, athletes[3].name, athletes[3].pace);
    std::thread swimmer5(swim, athletes[4].name, athletes[4].pace);
    std::thread swimmer6(swim, athletes[5].name, athletes[5].pace);

    swimmer1.join();
    swimmer2.join();
    swimmer3.join();
    swimmer4.join();
    swimmer5.join();
    swimmer6.join();

    std::sort(race_results.begin(), race_results.end(), race_result_compare);

    for(auto i=race_results.begin();i!=race_results.end();i++)
    {
        std::cout << i->name << " " << i->pace<< std::endl;
    }

    return 0;
}