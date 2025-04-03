import { useMutation } from '@tanstack/react-query';
import { toast } from 'sonner';

import { getStarlinkInfo } from '@/actions/getStarlinkInfo';

export const useStarlinkInfo = () => {
	return useMutation({
		mutationFn: getStarlinkInfo,
		onSuccess: (_, starlinkId) => {
			toast.success(
				`Retrieved information about starlink with ID: ${starlinkId}}`,
			);
		},
		onError: (error) => {
			toast.error(error.message ?? 'An unknown error occured.');
		},
	});
};
