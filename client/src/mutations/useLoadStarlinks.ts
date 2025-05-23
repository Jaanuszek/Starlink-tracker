import { useMutation } from '@tanstack/react-query';
import { toast } from 'sonner';

import { loadStarlinks } from '@/actions/loadStarlinks';
import { getQueryClient } from '@/lib/queryClient';
import { QueryKeys } from '@/constants/actions.const';
import { LoadedStarlinkInfo } from '@/actions/getLoadedStarlinksInfo';

export const useLoadStarlinks = () => {
	const queryClient = getQueryClient();

	return useMutation({
		mutationFn: (starlinksIds: string) => loadStarlinks(starlinksIds),
		onSuccess: () => {
			queryClient.invalidateQueries({
				queryKey: [QueryKeys.GET_LOADED_STARLINKS_INFO],
			});
			toast.success('Starlinks have been loaded correctly');
		},
		onMutate: async (starlinkIds) => {
			await queryClient.cancelQueries({
				queryKey: [QueryKeys.GET_LOADED_STARLINKS_INFO],
			});

			const previousState = queryClient.getQueryData<LoadedStarlinkInfo[]>([
				QueryKeys.GET_LOADED_STARLINKS_INFO,
			]);

			const ids = starlinkIds.split(',').map((id) => parseInt(id.trim(), 10));

			const starlinks = [
				...(previousState ?? []),
				...ids.map((id) => ({
					id,
					visible: true,
					trajectoryVisible: false,
				})),
			];

			queryClient.setQueryData<LoadedStarlinkInfo[]>(
				[QueryKeys.GET_LOADED_STARLINKS_INFO],
				() => starlinks,
			);
		},
		onError: (error) => {
			toast.error(error.message ?? 'An unknown error occured.');
		},
	});
};
