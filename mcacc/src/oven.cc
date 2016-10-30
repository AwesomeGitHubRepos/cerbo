#include <algorithm>

#include "oven.hpp"

#include "cgt.hpp"
#include "epics.hpp"
#include "etb.hpp"
#include "etrans-aug.hpp"
#include "gaap.hpp"
#include "posts.hpp"
#include "stend.hpp"
#include "wiegley.hpp"
#include "yproc.hpp"


using namespace std;

void oven::load_inputs()
{
	user_inputs = read_inputs();
}

void oven::fetch()
{
	for(const auto& y:process_yahoos(user_inputs))
		fetched_yahoos.insert(y);
}

period oven::curr_period() const
{
	period p = user_inputs.p;
	if(m_vm.count("end")>0) p.end_date = m_vm.at("end");
	if(m_vm.count("start")>0) p.start_date = m_vm.at("start");
	return p;
}
void oven::process(bool do_wiegley)
{
	yahoo_ts all_yahoos;
	const inputs_t& inps = user_inputs;
	set_union(inps.yahoos.begin(), inps.yahoos.end(),
			fetched_yahoos.begin(), fetched_yahoos.end(),
			inserter(all_yahoos, all_yahoos.begin()));
	const period& perd = curr_period();
	const stend_ts stends = stend_main(all_yahoos, perd);
	const detran_cs augetrans = eaug_main(inps.etrans, stends,
			perd);
	const folio_cs folios = epics_main(augetrans, stends);
	const post_ts posts = posts_main(inps.naccs, inps.ntrans, folios, perd);
	etb_main(user_inputs.naccs, posts);
	gaap_main(inps.naccs, perd);
	cgt(inps.etrans, perd);

	if(do_wiegley) wiegley(inps.etrans, inps.ntrans, inps.yahoos);

}
