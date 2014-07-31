#ifndef __fpga_components_hpp__
#define __fpga_components_hpp__

#include <vector>
#include <string>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <cassert>

using namespace std;

namespace fpga {
    class Component;

    class Tile {
        friend class Model;
        string name;

        Tile(string name) :
            name(name)
        { }

    public:
        const string& getName() const {
            return name;
        }
    };

    class Primitive {
        friend class Model;
        string name, typeName;
        unordered_set<Component*> components;

        Primitive(string name, string typeName) :
            name(name),
            typeName(typeName)
        {
        }

        Primitive(string name) :
            name(name)
        {
        }

    public:
        const string& getName() const {
            return name;
        }

        void setTypeName(string typeName) {
            this->typeName = typeName;
        }

    protected:
        void registerComponent(Component* component) {
            components.insert(component);
        }
    };

    class Model {
        string name;
        unordered_map<string, Component*> globalNameIndex;
        unordered_map<string, Tile> tiles;
        unordered_map<string, Primitive> primitives;
    public:
        Model(string name) :
            name(name)
        { }

        Tile& getTile(string name) {
            auto f = tiles.find(name);
            if (f == tiles.end()) {
                tiles.insert(make_pair(name, Tile(name)));
                f = tiles.find(name);
                assert(f != tiles.end());
            }
            return f->second;
        }

        Primitive& getPrimitive(string name) {
            auto f = primitives.find(name);
            if (f == primitives.end()) {
                primitives.insert(make_pair(name, Primitive(name)));
                f = primitives.find(name);
                assert(f != primitives.end());
            }
            return f->second;
        }

        Component* createComponent(string name, string tile, string primitive,
                                   const set<string>& cfgOpts,
                                   const set<string>& inputPins,
                                   const set<string>& outputPins);
    protected:
        friend class Component;
        void registerComponent(string name, Component* component) {
            globalNameIndex[name] = component;
        }
    };

    class Pin {
        Primitive* primitive;
        Component* component;
        Pin* srcPin;
        set<Pin*> dstPins;

    public:
        Pin(Primitive* prim, Pin* srcPin = NULL) :
            primitive(prim),
            component(NULL),
            srcPin(srcPin)
        { }

        Pin(Component* component, Pin* srcPin = NULL) :
            primitive(NULL),
            component(component),
            srcPin(srcPin)
        { }

        void setSrc(Pin* srcPin) {
            this->srcPin = srcPin;
        }

        void addDst(Pin* dstPin) {
            dstPins.insert(dstPin);
        }

        const set<Pin*>& getDst() const {
            return dstPins;
        }

        bool isInput() const {
            return srcPin != NULL;
        }

        bool isOutput() const {
            return dstPins.size() > 0; 
        }

        bool isPrimitivePin() const {
            return primitive != NULL;
        }

        bool isComponentPin() const {
            return component != NULL;
        };
    };

    class Component {
        string localName;
        string globalName;
        Model& model;
        Tile& tile;
        Primitive& primitive;
    public:
        Component(Model& model, string name, string tileName, string primName) :
            localName(name),
            model(model),
            tile(model.getTile(tileName)),
            primitive(model.getPrimitive(primName))
        {
            if (primName != "")
                this->globalName = this->primitive.getName() + " " + name;
            else
                this->globalName = this->tile.getName() + " " + name;
            this->model.registerComponent(name, this);
        }

        string getGlobalName() const {
            return globalName;
        }

        string getLocalName() const {
            return localName;
        }

        virtual ~Component() { }
        
        virtual const vector<string>& getCfgOpts() const = 0;
        virtual string getCfg(string name) = 0;
    };


#define STD_STUFF() \
    friend class Model; \
private: \
    const static vector<string> cfgOpts; \
public: \
    virtual const vector<string>& getCfgOpts() const { return cfgOpts; }


    class CombinatorialLogic : public Component {
    };

    class MiscLogic : public Component {
        vector<string> cfgOpts;

    public:
        virtual const vector<string>& getCfgOpts() const { return cfgOpts; }
    };

    class Clocked : public Component {
    };

    class FlipFlop : public Clocked {
        STD_STUFF()
    };

    class Latch : public Clocked {
        STD_STUFF();
    };

    class MiscClocked : public Clocked {
        vector<string> cfgOpts;

    public:
        virtual const vector<string>& getCfgOpts() const { return cfgOpts; }
    };

    class RoutingResource : public Component {
    };

    class PIP : public RoutingResource {
        STD_STUFF();
    };

    class Mux : public RoutingResource {
        STD_STUFF();
    };
};

#endif //__fpga_components_hpp__
