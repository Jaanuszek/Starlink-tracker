import { useMutation } from '@tanstack/react-query';
import { toast } from 'sonner';

import { loadStarlinks } from '@/actions/loadStarlinks';

export const useLoadStarlinks = () => {
	return useMutation({
		mutationFn: (starlinksIds: string) => loadStarlinks(starlinksIds),
		onSuccess: () => {
			toast.success('Starlinks have been loaded correctly');
		},
		onError: (error) => {
			toast.error(error.message ?? 'An unknown error occured.');
		},
	});
};
