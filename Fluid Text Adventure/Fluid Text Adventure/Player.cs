using FluidHTN;

namespace Fluid_Text_Adventure
{
    public class Player
    {
        private Planner<AIContext> _planner;
        private Domain<AIContext> _domain;
        private AIContext _context;

        public AIContext Context => _context;

        public Player(Domain<AIContext> domain)
        {
            _domain = domain;
            _planner = new Planner<AIContext>();
            _context = new AIContext(this);
            _context.Init();
        }

        public void Think()
        {
            var startGoal = _context.GetGoal();
            while (_context.GetGoal() == startGoal)
            {
                _planner.Tick(_domain, _context);
            }
        }
    }
}