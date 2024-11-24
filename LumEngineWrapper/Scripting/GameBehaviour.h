#pragma once
#include "../LumTypes/GameBehaviour/GameBehaviour.h"
#include <vcclr.h>

namespace LumScripting
{
    namespace Scripting
    {
        ref class GameBehaviour;

        class BridgeGameBehaviour : public IGameBehaviour {
        private:
            gcroot<GameBehaviour^> managed;

        public:
            BridgeGameBehaviour(GameBehaviour^ owner) : managed(owner) {}

            virtual void OnStart() override;
            virtual void OnRun() override;
        };

        public ref class GameBehaviour abstract {
        private:
            IGameBehaviour* native;

        public:
            GameBehaviour() : native(new BridgeGameBehaviour(this)) {}

            virtual void OnStart() abstract;
            virtual void OnRun() abstract;

            !GameBehaviour() {
                if (native) {
                    delete native;
                    native = nullptr;
                }
            }
        };

        void BridgeGameBehaviour::OnStart() {
            managed->OnStart();
        }

        void BridgeGameBehaviour::OnRun() {
            managed->OnRun();
        }
    }
}