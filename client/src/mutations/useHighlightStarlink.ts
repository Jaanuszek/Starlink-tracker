import { useMutation } from '@tanstack/react-query';
import { toast } from 'sonner';

import { highlightStarlink } from '@/actions/highlightStarlink';

export const useHighlightStarlink = () => {
	return useMutation({
		mutationFn: (startlinkId: string) => highlightStarlink(startlinkId),
		onSuccess: (_, starlinkId) => {
			toast.success(`Highlight starlink with ID: ${starlinkId}`);
		},
		onError: (error) => {
			toast.error(error.message ?? 'An unknown error occured.');
		},
	});
};
