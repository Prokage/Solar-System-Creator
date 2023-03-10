#include <iostream>
#include <vector>
#include <string>
#include <math.h>
using namespace std;
const double G = 6.67e-11; // gravitational constant
// Define classes for celestial bodies
class CelestialBody {
public:
    string name;
    double mass;
    double radius;
    vector<CelestialBody*> satellites; // new member variable to hold satellites
    CelestialBody(string name,  double mass, double radius) {
        this->name = name;
        this->mass = mass;
        this->radius = radius;
    }
    virtual ~CelestialBody() {} // polymorphic reasons
	double calculateOrbitalPeriod(double distanceFromCentralBody, double centralBodyMass) {
	    double a = distanceFromCentralBody * 1000; // convert distance from km to m
	    double T = sqrt((4 * pow(3.14159, 2) * pow(a, 3)) / (G * centralBodyMass)); // calculate the orbital period in seconds
	    T /= 86400; // convert the orbital period to days
	    cout << "The orbital period of this body with respect to the central celestial body is: " << T << " days."<<endl;
	    cout << "This is calculated using Keplers third law: T^2 = (4 * pi ^ 2 * a^3)/(G*M)"<<endl;
	    cout << "The important factors that affect this value are a: the distance from the central body, M: the mass of the central body"<<endl;
	    return T;
	}
	void addBody(CelestialBody* satellite) {
        satellites.push_back(satellite);
    }
    
    double calculateEscapeVelocity(){
    	return sqrt(2*G*mass/(radius*1000)); // kilometres per second
	}
	
	double calculateSurfaceGravity(){
		return G* mass/pow(radius * 1000, 2); // metres per second squared
	}
};
CelestialBody* sun;
class Planet : public CelestialBody {
public:
	double distanceFromStar;
    double orbitalPeriod;
    Planet(string name, double mass, double radius, double distanceFromStar) : CelestialBody(name, mass, radius) {
		this->distanceFromStar=distanceFromStar;
		this->orbitalPeriod = calculateOrbitalPeriod(distanceFromStar, sun->mass);
	}
};

class Moon : public Planet {
public:
    Moon(string name, double mass, double radius, double distanceFromPlanet, double parentPlanetMass) : Planet(name, mass, radius, distanceFromPlanet) {
	this->orbitalPeriod = calculateOrbitalPeriod(distanceFromPlanet, parentPlanetMass);
	}
};

// Define the solar system class
class SolarSystem {
public:
	CelestialBody* sun;
    vector<CelestialBody*> bodies;
    SolarSystem() {
    	sun = new CelestialBody("Sun", 1.99e30, 696340);
    	bodies.push_back(sun);
	}
    void addBody(CelestialBody* body) {
        bodies.push_back(body);
    }
    
    void listBodies() {
		for (int i = 0; i < bodies.size(); i++) {
	        CelestialBody* body = bodies[i];
	        cout << body->name << " (";
	        if (dynamic_cast<Planet*>(body)) {
	            cout << "Planet): mass = " << body->mass << " kg, radius = " << body->radius << " km, distance from star = " << dynamic_cast<Planet*>(body)->distanceFromStar << " km, orbital period = " << dynamic_cast<Planet*>(body)->orbitalPeriod << " days";
	        } else if (dynamic_cast<Moon*>(body)) {
	            cout << "Moon): mass = " << body->mass << " kg, radius = " << body->radius << " km, distance from planet = " << dynamic_cast<Moon*>(body)->distanceFromStar << " km, orbital period = " << dynamic_cast<Moon*>(body)->orbitalPeriod << " days";
	        } else {
	            cout << "Celestial Body): mass = " << body->mass << " kg, radius = " << body->radius << " km";
	        }
	        if (!body->satellites.empty()) {
	            cout << ", orbiting: ";
	            for (int j = 0; j < body->satellites.size(); j++) {
	                CelestialBody* satellite = body->satellites[j];
	                cout << satellite->name << " (" << satellite->radius << " km away)";
	                if (j < body->satellites.size() - 1) {
	                    cout << ", ";
	                }
	            }
	        }
	        cout << endl;
	    }
    }
    Planet* getPlanet(string planetName) {
        for (int i = 0; i < bodies.size(); i++) {
            Planet* planet = dynamic_cast<Planet*>(bodies[i]);
            if (planet != nullptr && planet->name == planetName) {
                return planet;
            }
        }
        return nullptr;
    }
	void updateSun(string name, double mass, double radius) {
	    sun->name = name;
	    sun->mass = mass;
	    sun->radius = radius;
	}
	void removeBody(string name){
		for (int i = 0; i < bodies.size(); i++) {
	        if (bodies[i]->name == name) {
	            // Remove the body and all of its satellites
	            delete bodies[i];
	            bodies.erase(bodies.begin() + i);
	            i--;
	        } else if (!bodies[i]->satellites.empty()) {
	            // Recursively remove any satellites of the body
	            for (int j = 0; j < bodies[i]->satellites.size(); j++) {
	                if (bodies[i]->satellites[j]->name == name) {
	                    delete bodies[i]->satellites[j];
	                    bodies[i]->satellites.erase(bodies[i]->satellites.begin() + j);
	                    j--;
	                }
	            }
	        }
	    }
	}
};



int main() {
    // Create a solar system
    SolarSystem solarSystem;
    
    sun = solarSystem.sun;

    // Main menu loop
    char choice = ' ';
    while (true) {
        // Display options
		cout << "What would you like to do?" << endl;
		cout << "1. Add a body" << endl;
		cout << "2. List all bodies" << endl;
		cout << "3. Remove a body" << endl;
		cout << "4. Calculate Escape Velocity for a body" << endl;
		cout << "5. Calculate Surface Gravity of a body" << endl;
		cout << "6. Exit" << endl;
		cout << "Enter your choice (1-6): ";
		cin >> choice;
		cout<<endl;
        // Process user input
        if (choice == '1') {
            // Add a body
            char bodyType = ' ';
            cout << "What kind of body would you like to add?" << endl;
            cout << "1. Sun (Updates current sun, only 1 allowed)" << endl;
            cout << "2. Planet" << endl;
            cout << "3. Moon" << endl;
            cout << "Enter your choice (1-3): ";
            cin >> bodyType;
			cout<<endl;
            if (bodyType == '1') {
            	// Update the sun
			    string sunName;
			    double sunMass, sunRadius;
			    cout << "Enter the new name of the sun: ";
			    cin >> sunName;
			    cout<<"For context, the mass of our sun is 1.989e30"<<endl;
			    cout << "Enter the new mass of the sun in kg: ";
			    cin >> sunMass;
			    cout<<"For context, the radius of our sun is 696340 km "<<endl;
			    cout << "Enter the new radius of the sun in km: ";
			    cin >> sunRadius;
			    solarSystem.updateSun(sunName, sunMass, sunRadius);
			    cout << "Sun updated." << endl;
            } else if (bodyType == '2') {
                // Add a planet
                string planetName;
                double planetMass, planetRadius, planetDistance;
                cout << "Enter the name of the planet: ";
                cin >> planetName;
                cout<<"For context, the mass of our Earth is 5.972e24"<<endl;
                cout << "Enter the mass of " << planetName << " (in kg): ";
                cin >> planetMass;
                cout<<"For context, the radius of our Earth is 6400 km"<<endl;
                cout << "Enter the radius of " << planetName << " (in km): ";
                cin >> planetRadius;
                cout <<"For context, our Earth is 1.5e8 km from our sun"<<endl;
                cout << "Enter the distance of " << planetName << " from the sun (in km): ";
                cin >> planetDistance;
                Planet* planet = new Planet(planetName, planetMass, planetRadius, planetDistance);
                solarSystem.addBody(planet);
                cout << "Planet added." << endl;
            } else if (bodyType == '3') {
                // Add a moon
                string moonName, planetName;
                double moonMass, moonRadius, moonDistance, moonPeriod;
                cout << "Enter the name of the moon: ";
                cin >> moonName;
                cout<<"For context, the mass of Earths moon is 7.347e22"<<endl;
                cout << "Enter the mass of " << moonName << " (in kg): ";
                cin >> moonMass;
                cout<<"For context, the radius of Earths moon is 1737km"<<endl;
                cout << "Enter the radius of " << moonName << " (in km): ";
                cin >> moonRadius;
                cout << "Enter the distance of " << moonName << " from its parent planet (in km): ";
                cin >> moonDistance;
                cout << "Enter the name of the parent planet: ";
                cin >> planetName;
                Planet* planet = solarSystem.getPlanet(planetName);
                if (planet == nullptr) {
                    cout << "Could not find planet named " << planetName << "." << endl;
                } else {
                    Moon* moon = new Moon(moonName, moonMass, moonRadius, moonDistance, planet->mass);
                    planet->addBody(moon);
                    cout << "Moon added." << endl;
                }
            } else {
                cout << "Invalid choice." << endl;
            }
        } else if (choice == '2') {
            // List all bodies
            cout << "The bodies in the solar system are: " << endl;
            solarSystem.listBodies();
        } else if (choice == '3'){
			// Remove a body
		    string bodyName;
		    cout << "Enter the name of the body to remove: ";
		    cin >> bodyName;
		    solarSystem.removeBody(bodyName);
		    cout << "Body removed." << endl;
		} else if (choice == '4') {
		    // Calculate escape velocity of a body
		    cout<<"Escape velocity is the velocity an object must reach before it overcomes the strength of the gravity of the body it is escaping from"<<endl;
		    cout<<"The equation for this is sqrt(2*G*M/r), M is the mass of the body you are escaping from and r is how far from the centre of that body you are"<<endl;
		    string bodyName;
		    cout << "Enter the name of the body: ";
		    cin >> bodyName;
		    CelestialBody* body = solarSystem.getPlanet(bodyName);
		    if (body == nullptr) {
		        cout << "Could not find body named " << bodyName << "." << endl;
		    } else {
		        double escapeVelocity = body->calculateEscapeVelocity();
		        cout << "The escape velocity of " << body->name << " at its surface is " << escapeVelocity << " km/s." << endl;
		    }
			} else if (choice == '5') {
		    // Calculate surface gravity of a body
		    string bodyName;
		    cout << "Enter the name of the body: ";
		    cin >> bodyName;
		    CelestialBody* body = solarSystem.getPlanet(bodyName);
		    if (body == nullptr) {
		        cout << "Could not find body named " << bodyName << "." << endl;
		    } else {
		        double surfaceGravity = body->calculateSurfaceGravity();
		        cout << "The surface gravity of " << body->name << " is " << surfaceGravity << " m/s^2." << endl;
		    }
		}
		 else if (choice == '6') {
            // Exit program
            cout << "Exiting program." << endl;
            break;
        } else {
            cout << "Invalid choice." << endl;
        }
    }

    // Clean up memory
    for (int i = 0; i < solarSystem.bodies.size(); i++) {
        delete solarSystem.bodies[i];
    }

    return 0;
}
