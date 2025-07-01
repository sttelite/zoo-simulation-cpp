#include <cmath>
#include <iostream>
#include <map>
#include <string>
#include <vector>

class Food {
 private:
  std::string name_;
  int calories_;

 public:
  Food(std::string name, int calories) : name_(name), calories_(calories) {}
  int getFoodCalories() const { return calories_; }
  std::string getFoodName() const { return name_; }
};

class Animal {
 protected:
  std::string name_;
  int calories_;
  std::string preferredFood_;
  Food* eatenFood_ = nullptr;
  int dailyCalories_;
  bool dead_ = false;

 public:
  Animal(std::string name, Food* eatenFood, int calories, int dailyCalories)
      : name_(name),
        eatenFood_(eatenFood),
        calories_(calories),
        dailyCalories_(dailyCalories) {}

  virtual void setPreferredFood(std::string food) { preferredFood_ = food; }

  virtual void setEatenFood(Food* food) { eatenFood_ = food; }

  virtual Food* getEatenFood() const { return eatenFood_; }
  virtual void simulateDay() {
    if (preferredFood_ == eatenFood_->getFoodName()) {
      calories_ += eatenFood_->getFoodCalories();
    } else {
      calories_ += (eatenFood_->getFoodCalories()) / 2;
    }
    calories_ -= dailyCalories_;
    if (calories_ < 0) {
      dead_ = true;
    }
  }

  virtual std::string getName() const { return name_; }

  virtual std::string getType() const { return "Animal"; }

  virtual int getCalories() const { return calories_; }

  bool isDead() const { return dead_; }

  virtual ~Animal() {}

};

class Cat : public Animal {
 public:
  Cat(std::string name, Food* eaten_food, int calories, int daily_calories)
      : Animal(name, eaten_food, calories, daily_calories) {
    preferredFood_ = "Milk";
  }

  std::string getType() const override { return "Cat"; }

  ~Cat() { std::cout << "Cat " << name_ << " died\n"; }
};

class Dog : public Animal {
 public:
  Dog(std::string name, Food* eaten_food, int calories, int daily_calories)
      : Animal(name, eaten_food, calories, daily_calories) {
    preferredFood_ = "Meat";
  }

  std::string getType() const override { return "Dog"; }

  ~Dog() { std::cout << "Dog " << name_ << " died\n"; }
};

class Parrot : public Animal {
 public:
  Parrot(std::string name, Food* eaten_food, int calories, int daily_calories)
      : Animal(name, eaten_food, calories, daily_calories) {
    preferredFood_ = "Apples";
  }

  std::string getType() const override { return "Parrot"; }

  ~Parrot() { std::cout << "Parrot " << name_ << " died\n"; }
};

class Zoo {
 private:
  std::vector<Animal*> animals;

 public:
  void addAnimal(Animal* animal) { animals.push_back(animal); }

  void feedAll(Food* food) {
    for (auto animal : animals) {
      animal->setEatenFood(food);
    }
  }

  void simulateDay() {
    for (auto it = animals.begin(); it != animals.end();) {
      Animal* animal = *it;
      animal->simulateDay();
      if (animal->isDead()) {
        delete animal;
        it = animals.erase(it);
      } else {
        ++it;
      }
    }
  }

  bool isEmpty() { return animals.empty(); }

  void printStatus() {
    for (auto animal : animals) {
      std::cout << "Animal " << animal->getType() << " named "
                << animal->getName() << " has accumulated "
                << animal->getCalories()
                << " calories and currently is being fed "
                << (animal->getEatenFood())->getFoodName() << std::endl;
    }
  }
};

void runZooSimulation() {
  Food milk("Milk", 20);
  Food meat("Meat", 30);
  Food apple("Apples", 15);
  Food bread("Bread", 10);

  std::map<std::string, Food*> foodMap = {
      {"milk", &milk}, {"meat", &meat}, {"apples", &apple}, {"bread", &bread}};

  Zoo zoo;

  while (true) {
    std::cout << "\n=== Zoo Menu ===\n";
    std::cout << "1. Add animal\n2. Feed all animals\n3. Simulate day\n4. "
                 "Print status\n5. Exit\n> ";

    int choice;
    std::cin >> choice;
    std::cin.ignore();

    if (choice == 1) {
      std::string type, name;
      int calories, dailyCalories;
      std::string foodChoice;

      std::cout << "Enter animal type (cat/dog/parrot): ";
      std::getline(std::cin, type);
      std::cout << "Enter name: ";
      std::getline(std::cin, name);
      std::cout << "Enter starting calories: ";
      std::cin >> calories;
      std::cout << "Enter daily calorie needs: ";
      std::cin >> dailyCalories;
      std::cin.ignore();

      std::cout << "Choose food (milk/meat/apples/bread): ";
      std::getline(std::cin, foodChoice);

      if (foodMap.count(foodChoice) == 0) {
        std::cout << "Invalid food. Animal not added.\n";
        continue;
      }

      if (type == "cat") {
        zoo.addAnimal(
            new Cat(name, foodMap[foodChoice], calories, dailyCalories));
      } else if (type == "dog") {
        zoo.addAnimal(
            new Dog(name, foodMap[foodChoice], calories, dailyCalories));
      } else if (type == "parrot") {
        zoo.addAnimal(
            new Parrot(name, foodMap[foodChoice], calories, dailyCalories));
      } else {
        std::cout << "Unknown animal type.\n";
      }

    } else if (choice == 2) {
      std::string foodChoice;
      std::cout << "Choose food to feed all animals (milk/meat/apples/bread): ";
      std::getline(std::cin, foodChoice);
      if (foodMap.count(foodChoice)) {
        zoo.feedAll(foodMap[foodChoice]);
        std::cout << "All animals now eating " << foodChoice << "\n";
      } else {
        std::cout << "Invalid food.\n";
      }

    } else if (choice == 3) {
      zoo.simulateDay();
      zoo.printStatus();
      if (zoo.isEmpty()) {
        std::cout << "All animals died. Simulation over.\n";
        break;
      }

    } else if (choice == 4) {
      zoo.printStatus();

    } else if (choice == 5) {
      std::cout << "Exiting simulation.\n";
      break;

    } else {
      std::cout << "Invalid choice.\n";
    }
  }
}

int main() {
  runZooSimulation();

  std::cin.get();
  return 0;
}
