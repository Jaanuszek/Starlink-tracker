import { useQuery } from '@tanstack/react-query';

import { getLoadedStarlinksInfo } from '@/actions/getLoadedStarlinksInfo';
import { QueryKeys } from '@/constants/actions.const';

export const useGetLoadedStarlinksInfo = () => {
	return useQuery({
		queryKey: [QueryKeys.GET_LOADED_STARLINKS_INFO],
		queryFn: getLoadedStarlinksInfo,
	});
};
