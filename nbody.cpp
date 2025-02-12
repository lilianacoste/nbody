#include<iostream>
#include<cmath>
#include<random>
#include<vector>
#include<fstream>
#include<cstdlib>
#include<string>
#include <algorithm>


//gravitational constant
const double G = 6.67430e-11; //e-11 = 10^-11
const double SOFTENING = 1e-9;
struct particle {
    double mass; // mass
    double x, y, z; // position
    double vx, vy, vz; // velocity
    double fx, fy, fz; // force
  
};
//function to randomly initialize the particles
void intializeParticlesRand(std::vector<particle>& particles, int n) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0, 1);
    particles.resize(n);
    for (int i = 0; i < n; i++) {
        particles[i].mass = dis(gen);
        particles[i].x = dis(gen);
        particles[i].y = dis(gen);
        particles[i].z = dis(gen);
        particles[i].vx = dis(gen);
        particles[i].vy = dis(gen);
        particles[i].vz = dis(gen);
        particles[i].fx = 0.0;
        particles[i].fy = 0.0;
        particles[i].fz = 0.0;
       
    }
}
//predefined configuration of particles
void initializeParticlesPre(std::vector<particle>& particles) {
    particles.emplace_back(1.989e30, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
    particles.emplace_back(5.972e24, 1.496e11, 0.0, 0.0, 29783.0, 0.0, 0.0, 0.0, 0.0);
    particles.emplace_back(7.348e22, 1.496e11+3.844e8, 0.0, 0.0, 29783.0+1022.0, 0.0, 0.0, 0.0, 0.0);
   
    
}
//reading the particles from the file
void initializeParticlesFile(std::vector<particle>& particles, const std::string& filename) {
    std::ifstream file(filename);
    if(!file){
        std::cerr << "Error opening file" << std::endl;
        exit(1);
    }
    int n;
    file >> n;
    particles.resize(n);
    for(int i = 0; i < n; i++) {
        file >> particles[i].mass >> particles[i].x >> particles[i].y >> particles[i].z >> particles[i].vx >> particles[i].vy >> particles[i].vz >> particles[i].fx >> particles[i].fy >> particles[i].fz;
    }
    file.close();
}
void calculateForces(std::vector<particle>& particles, double G, double softening) {
    int n = particles.size();
    for(int i =0; i<n; i++) {
        particles[i].fx = 0.0;
        particles[i].fy = 0.0;
        particles[i].fz = 0.0;
    }
    for(int i=0;i<n;i++){
        for(int j=i+1;j<n;j++){
            double dx = particles[j].x - particles[i].x;
            double dy = particles[j].y - particles[i].y;
            double dz = particles[j].z - particles[i].z;
            
            double r2=dx*dx+dy*dy+dz*dz+SOFTENING*SOFTENING;
            double r = sqrt(r2);

            double F = G*particles[i].mass*particles[j].mass/r2;

            double Fx = F*dx/r;
            double Fy = F*dy/r;
            double Fz = F*dz/r;

            particles[i].fx += Fx;
            particles[i].fy += Fy;
            particles[i].fz += Fz;

            particles[j].fx -= Fx;
            particles[j].fy -= Fy;
            particles[j].fz -= Fz;
        }
    }
}
void updateVelocities(std::vector<particle>& particles, double dt) {
    int n = particles.size();
    for(int i = 0; i < n; i++) {
        if(particles[i].mass == 0.0) {
            continue;
        }
        particles[i].vx += particles[i].fx*dt/particles[i].mass;
        particles[i].vy += particles[i].fy*dt/particles[i].mass;
        particles[i].vz += particles[i].fz*dt/particles[i].mass;
    }
}
void updatePositions(std::vector<particle>& particles, double dt) {
    int n = particles.size();
    for(int i = 0; i < n; i++) {
        particles[i].x += particles[i].vx*dt;
        particles[i].y += particles[i].vy*dt;
        particles[i].z += particles[i].vz*dt;
    }
}
void outputState(const std::vector<particle>& particles) {
    std::cout << particles.size() << std::endl;  // Print number of particles first
    for (const auto& p : particles) {
        std::cout << p.mass << " " << p.x << " " << p.y << " " << p.z << " "
                  << p.vx << " " << p.vy << " " << p.vz << " "
                  << p.fx << " " << p.fy << " " << p.fz << std::endl;
    }
}

void simulate(std::vector<particle>& particles, double dt, int numSteps) {
    for(int step =0; step < numSteps; step++) {
       calculateForces(particles, G, SOFTENING);
         updateVelocities(particles, dt);
         updatePositions(particles, dt);
         if(step % 10 == 0) {
             outputState(particles);
         }
    }
}


void parseArguments(int argc, char* argv[], int& numParticles, double& dt, int& numIterations, int& dumpFreq, std::string& filename,bool& usePredefined) {
    if(argc < 5) {
        std::cerr << "Usage: " << argv[0] << " numParticles dt numIterations dumpFreq [filename]" << std::endl;
        exit(1);
    }
   std::string arg1 = argv[1];
   if(arg1 == "predefined"){
    usePredefined = true;
    numParticles = 3;
    filename = "";
   }
   else if(!arg1.empty() && std::all_of(arg1.begin(), arg1.end(), ::isdigit))
{
   usePredefined = false;
   numParticles = std::stoi(argv[1]);
   filename = "";
   }
   else{
    usePredefined = false;
    numParticles = 0;
    filename = arg1;
   }
    dt = std::stod(argv[2]);
    numIterations = std::stoi(argv[3]);
    dumpFreq = std::stoi(argv[4]);
   
}
int main(int argc, char*argv[]){
    int numParticles;
    double dt;
    int numIterations;
    int dumpFreq;
    std::string filename;
    bool usePredefined = false;

    parseArguments(argc, argv, numParticles, dt, numIterations, dumpFreq, filename, usePredefined);
    std::vector<particle> particles;
    if(usePredefined){
        initializeParticlesPre(particles);
    }
    else if(filename != ""){
        initializeParticlesFile(particles, filename);
    }
    else{
        intializeParticlesRand(particles, numParticles);
    }
    
    simulate(particles, dt, numIterations);
    
    return 0;
}