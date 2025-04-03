import { useMutation } from '@tanstack/react-query';
import { toast } from 'sonner';

import { disableStarlink } from '@/actions/disableStarlink';

export const useDisableStarlink = () => {
	return useMutation({
		mutationFn: (startlinkId: string) => disableStarlink(startlinkId),
		onSuccess: (_, starlinkId) => {
			toast.success(`Hide starlink with ID: ${starlinkId}`);
		},
		onError: (error) => {
			toast.error(error.message ?? 'An unknown error occured.');
		},
	});
};
