using FluidHTN;

namespace Fluid_Text_Adventure
{
    public class Player
    {
        private Planner<AIContext> _planner;
        private AIContext _context;

        public AIContext Context => _context;

        public Player()
        {
            _planner = new Planner<AIContext>();
            _context = new AIContext(this);
            _context.Init();
        }

        public void Think(Domain<AIContext> domain)
        {
            var startGoal = _context.GetGoal();
            while (_context.GetGoal() == startGoal)
            {
                _planner.Tick(domain, _context);
            }
        }
    }
}