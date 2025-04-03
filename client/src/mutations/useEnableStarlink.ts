import { useMutation } from '@tanstack/react-query';
import { toast } from 'sonner';

import { enableStarlink } from '@/actions/enableStarlink';

export const useEnableStarlink = () => {
	return useMutation({
		mutationFn: (startlinkId: string) => enableStarlink(startlinkId),
		onSuccess: (_, starlinkId) => {
			toast.success(`Show starlink with ID: ${starlinkId}`);
		},
		onError: (error) => {
			toast.error(error.message ?? 'An unknown error occured.');
		},
	});
};
