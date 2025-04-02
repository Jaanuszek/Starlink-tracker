import { useMutation } from '@tanstack/react-query';
import { toast } from 'sonner';

import { rotateCamera } from '@/actions/rotateCamera';

export const useRotateCamera = () => {
	return useMutation({
		mutationFn: rotateCamera,
		onSuccess: () => {
			toast.success('Camera rotated');
		},
		onError: (error) => {
			toast.error(error.message ?? 'An unknown error occured.');
		},
	});
};
