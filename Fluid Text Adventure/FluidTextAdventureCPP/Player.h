#pragma once
#include "CoreIncludes/Domain.h"
#include "Planners/Planner.h"
#include "DebugInterfaces/DecompositionLogEntry.h"
#include "Console.h"

#include "AIContext.h"

class Player : public std::enable_shared_from_this<Player>
{
private:
    SharedPtr<Planner> _planner;
    SharedPtr<class AIContext> _context;
public:
    SharedPtr<AIContext>& Context() { return _context; }

    Player()
    {
        _planner = MakeSharedPtr<class Planner>();
    }
    void Init()
    {
        _context = MakeSharedPtr<AIContext>(shared_from_this());
        _context->Init();
    }
    void Think(Domain& domain)
    {
        auto startGoal = _context->GetGoal();
        while (_context->GetGoal() == startGoal)
        {
            _planner->Tick<AIWorldState,uint8_t,AIWorldStateType>(domain, *_context);

            if (_context->LogDecomposition())
            {
                Console::WriteLine("---------------------- DECOMP LOG --------------------------"s);
                while (_context->DecompositionLog().size() > 0)
                {
                    auto entry = _context->DecompositionLog().front();
                    _context->DecompositionLog().pop();
                    auto depth = FluidHTN::Debug::DepthToString(entry.Depth);
                    Console::ForegroundColor(entry.Color);
                    Console::WriteLine(depth + entry.Name + ":"s + entry.Description);
                }
                Console::WriteLine("-------------------------------------------------------------");
            }
        }
    }
};
