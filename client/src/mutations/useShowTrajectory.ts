import { useMutation } from '@tanstack/react-query';
import { toast } from 'sonner';

import { showTrajectory } from '@/actions/showTrajectory';

export const useShowTrajectory = () => {
	return useMutation({
		mutationFn: (startlinkId: string) => showTrajectory(startlinkId),
		onSuccess: (_, starlinkId) => {
			toast.success(`Show starlink trajectory with ID: ${starlinkId}`);
		},
		onError: (error) => {
			toast.error(error.message ?? 'An unknown error occured.');
		},
	});
};
