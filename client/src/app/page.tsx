import { getQueryClient } from '@/lib/queryClient';
import HomeContent from '@/components/HomeContent';
import { QueryKeys } from '@/constants/actions.const';
import { getCountriesBorderVisibility } from '@/actions/getCountriesBorderVisibility';
import { dehydrate, HydrationBoundary } from '@tanstack/react-query';
import { getLoadedStarlinksInfo } from '@/actions/getLoadedStarlinksInfo';

export default async function Home() {
	const queryClient = getQueryClient();
	await queryClient.prefetchQuery({
		queryKey: [QueryKeys.GET_COUNTRIES_BORDER_VISIBILITY],
		queryFn: getCountriesBorderVisibility,
	});
	await queryClient.prefetchQuery({
		queryKey: [QueryKeys.GET_LOADED_STARLINKS_INFO],
		queryFn: getLoadedStarlinksInfo,
	});

	return (
		<HydrationBoundary state={dehydrate(queryClient)}>
			<div className='container mx-auto'>
				<HomeContent />
			</div>
		</HydrationBoundary>
	);
}
