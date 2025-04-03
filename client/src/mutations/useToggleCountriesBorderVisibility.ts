import { useMutation } from '@tanstack/react-query';
import { toast } from 'sonner';

import { toggleCountriesBorderVisibility } from '@/actions/toggleCountriesBorderVisibility';
import { QueryKeys } from '@/constants/actions.const';
import { getQueryClient } from '@/lib/queryClient';

export const useToggleCountriesBorderVisibility = () => {
	const queryClient = getQueryClient();

	return useMutation({
		mutationFn: toggleCountriesBorderVisibility,
		onSuccess: ({ isCountriesBorderVisible }) => {
			queryClient.invalidateQueries({
				queryKey: [QueryKeys.GET_COUNTRIES_BORDER_VISIBILITY],
			});
			toast.success(
				`Display of country borders has been ${
					isCountriesBorderVisible ? 'enabled' : 'disabled'
				}`,
			);
		},
		onMutate: async () => {
			await queryClient.cancelQueries({
				queryKey: [QueryKeys.GET_COUNTRIES_BORDER_VISIBILITY],
			});

			const previousState = queryClient.getQueryData<boolean>([
				QueryKeys.GET_COUNTRIES_BORDER_VISIBILITY,
			]);
			queryClient.setQueryData<boolean>(
				[QueryKeys.GET_COUNTRIES_BORDER_VISIBILITY],
				() => !previousState,
			);
		},
		onError: (error) => {
			toast.error(error.message ?? 'An unknown error occured.');
		},
	});
};
