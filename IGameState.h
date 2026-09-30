class IGameState {
public:
    virtual ~IGameState() = default;
    virtual void init() = 0;
    virtual void update(float deltaTime) = 0;
    virtual void render() = 0;
};

//Aislar las reglas y dinamicas de cada nivel