import { useQuery } from '@tanstack/react-query';

import { getCountriesBorderVisibility } from '@/actions/getCountriesBorderVisibility';
import { QueryKeys } from '@/constants/actions.const';

export const useGetCountriesBorderVisibility = () => {
	return useQuery({
		queryKey: [QueryKeys.GET_COUNTRIES_BORDER_VISIBILITY],
		queryFn: getCountriesBorderVisibility,
	});
};
