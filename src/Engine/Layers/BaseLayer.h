#include "Layer.h"

class BaseLayer : public Layer {
   public:
    virtual void OnAttach() override;
    virtual void OnDetach() override;
    virtual void Update() override;
    virtual void OnEvent(Event& event);

   private:
};