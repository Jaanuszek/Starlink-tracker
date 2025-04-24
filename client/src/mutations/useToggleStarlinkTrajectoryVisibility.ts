import { LoadedStarlinkInfo } from '@/actions/getLoadedStarlinksInfo';
import { toggleStarlinkTrajectoryVisibility } from '@/actions/toggleStarlinkTrajectoryVisibility';
import { QueryKeys } from '@/constants/actions.const';
import { getQueryClient } from '@/lib/queryClient';
import { useMutation } from '@tanstack/react-query';
import { toast } from 'sonner';

export const useToggleStarlinkTrajectoryVisibility = () => {
	const queryClient = getQueryClient();

	return useMutation({
		mutationFn: toggleStarlinkTrajectoryVisibility,
		onSuccess: ({ starlinkId }) => {
			queryClient.invalidateQueries({
				queryKey: [QueryKeys.GET_LOADED_STARLINKS_INFO],
			});
			toast.success(`Toggle starlink ${starlinkId} visibility`);
		},
		onMutate: async (starlinkId) => {
			await queryClient.cancelQueries({
				queryKey: [QueryKeys.GET_LOADED_STARLINKS_INFO],
			});

			const previousState = queryClient.getQueryData<LoadedStarlinkInfo[]>([
				QueryKeys.GET_LOADED_STARLINKS_INFO,
			]);

			const starlinks = previousState?.map((starlink) => {
				if (starlink.id === starlinkId) {
					return {
						...starlink,
						trajectoryVisible: !starlink.trajectoryVisible,
					};
				}

				return starlink;
			});

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
