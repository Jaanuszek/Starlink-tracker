'use server';

import { fetcher } from '@/lib/fetcher';

type RotateCameraProps = {
	angleX: number;
	angleY: number;
};

export const rotateCamera = async (props: RotateCameraProps) => {
	const response = await fetcher<string>('RotateCamera', {
		body: props,
	});

	return response;
};
