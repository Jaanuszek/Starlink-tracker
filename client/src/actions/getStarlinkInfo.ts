'use server';

import { fetcher } from '@/lib/fetcher';

type StarlinkInfo = {
	id: number;
	satname: string;
	tleLine1: string;
	tleLine2: string;
	latitude: number;
	longitude: number;
	altitude: number;
	positionX: number;
	positionY: number;
	positionZ: number;
	speedX: number;
	speedY: number;
	speedZ: number;
	country: string;
};

export const getStarlinkInfo = async (starlinkId: string) => {
	const response = await fetcher<StarlinkInfo>(`GetStarlinkInfo/${starlinkId}`);

	return response;
};
