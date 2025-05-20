import { useMutation } from '@tanstack/react-query';
import { toast } from 'sonner';

import { unhighlightStarlink } from '@/actions/unhighlightStarlink';

export const useUnhighlightStarlink = () => {
	return useMutation({
		mutationFn: () => unhighlightStarlink(),
		onSuccess: (_, starlinkId) => {
			toast.success(`Unhighlight starlink with ID: ${starlinkId}`);
		},
		onError: (error) => {
			toast.error(error.message ?? 'An unknown error occured.');
		},
	});
};
