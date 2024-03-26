#include <iostream>
#include <vector>
#include <memory>

class Component : public std::enable_shared_from_this<Component> {
public:
    Component(std::string name) : name(name), visible(true) {}

    virtual void display(int level = 0) const {
        if (visible)
            std::cout << std::string(level, '-') << "Component: " << name << std::endl;
    }

    bool isVisible() const {
        return visible;
    }

    void setVisible(bool isVisible) {
        visible = isVisible;
    }

    virtual std::shared_ptr<Component> getChild(int index) {
        return nullptr;
    }

    virtual std::shared_ptr<Component> getParent() {
        return parent.lock();
    }

    virtual void setParent(std::shared_ptr<Component> newParent) {
        parent = newParent;
    }

    virtual void move(int newIndex) {}

    virtual std::shared_ptr<Component> getSibling(int index) {
        if (auto p = getParent())
            return p->getChild(index);
        return nullptr;
    }

    virtual ~Component() {}

protected:
    std::string name;
    bool visible;
    std::weak_ptr<Component> parent;
};

class Component;


class Container : public Component {
public:
    Container(std::string name) : Component(name) {}

    virtual void add(std::shared_ptr<Component> component) {
        components.push_back(component);
        component->setParent(shared_from_this());
    }


    virtual void display(int level = 0) const override {
        if (visible) {
            std::cout << std::string(level, '-') << "Container: " << name << std::endl;
            for (const auto& component : components) {
                component->display(level + 1);
            }
        }
    }

    virtual std::shared_ptr<Component> getChild(int index) override {
        if (index >= 0 && index < components.size()) {
            return components[index];
        }
        else {
            return nullptr;
        }
    }

    virtual std::shared_ptr<Component> getSibling(int index) override {
        auto p = getParent();
        if (p)
            return p->getChild(index);
        return nullptr;
    }

    virtual void move(int newIndex) override {
        
    }

    std::vector<std::shared_ptr<Component>> components;
};

class Window : public Container {
public:
    Window(std::string name) : Container(name) {}

    virtual std::shared_ptr<Component> getChild(int index) override {
        if (index >= 0 && index < components.size()) {
            return components[index];
        }
        else {
            return nullptr;
        }
    }

    virtual std::shared_ptr<Component> getParent() override {
        return std::shared_ptr<Component>(this);
    }

    virtual std::shared_ptr<Component> getSibling(int index) override {
        return nullptr;
    }
};

class Panel : public Container {
public:
    Panel(std::string name) : Container(name) {}

    virtual std::shared_ptr<Component> getChild(int index) override {
        if (index >= 0 && index < components.size()) {
            return components[index];
        }
        else {
            return nullptr;
        }
    }

    virtual std::shared_ptr<Component> getParent() override {
        return parent.lock();
    }

    virtual std::shared_ptr<Component> getSibling(int index) override {
        if (auto p = getParent()) 
            return p->getChild(index);
        return nullptr;
    }
};

class Label : public Component {
public:
    Label(std::string name) : Component(name) {}
};

class List : public Component {
public:
    List(std::string name) : Component(name) {}
};

class Button : public Component {
public:
    Button(std::string name) : Component(name) {}
};

class Line : public Component {
public:
    Line(std::string name) : Component(name) {}
};

int main() {
    // иерархия
    std::shared_ptr<Window> mainWindow = std::make_shared<Window>("Main Window");
    std::shared_ptr<Panel> panel1 = std::make_shared<Panel>("Panel 1");
    std::shared_ptr<Panel> panel2 = std::make_shared<Panel>("Panel 2");
    std::shared_ptr<Label> label1 = std::make_shared<Label>("Label 1");
    std::shared_ptr<Label> label2 = std::make_shared<Label>("Label 2");
    std::shared_ptr<Button> button1 = std::make_shared<Button>("Button 1");
    std::shared_ptr<Button> button2 = std::make_shared<Button>("Button 2");

    mainWindow->add(panel1);
    mainWindow->add(panel2);
    panel1->add(label1);
    panel1->add(button1);
    panel2->add(label2);
    panel2->add(button2);

    std::shared_ptr<Component> currentComponent = mainWindow;

    char choice;
    do {
        std::cout << "\nCurrent Component: ";
        currentComponent->display();

        std::cout << "\nOptions:\n";
        std::cout << "1. Select a child component by index\n";
        std::cout << "2. Select a component from the same container\n";
        std::cout << "3. Select the parent component\n";
        std::cout << "4. Move the current component within its container\n";
        std::cout << "5. Exit\n";
        std::cout << "Enter your choice: ";
        std::cin >> choice;

        switch (choice) {
        case '1': {
            int index;
            std::cout << "Enter the index of the child component: ";
            std::cin >> index;
            currentComponent = currentComponent->getChild(index);
            break;
        }
        case '2': {
            int index;
            std::cout << "Enter the index of the sibling component: ";
            std::cin >> index;
            currentComponent = currentComponent->getSibling(index);
            break;
        }
        case '3': {
            currentComponent = currentComponent->getParent();
            break;
        }
        case '4': {
            int newIndex;
            std::cout << "Enter the new index for the current component: ";
            std::cin >> newIndex;
            currentComponent->move(newIndex);
            break;
        }   
        case '5': {
            std::cout << "Exiting...\n";
            break;
        }
        default: {
            std::cout << "Invalid choice. Please enter again.\n";
        }
        }
    } while (choice != '5'); 

    return 0;
}
