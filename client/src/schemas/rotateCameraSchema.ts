import * as z from 'zod';

export const rotateCameraSchema = z.object({
	angleX: z
		.string()
		.refine(
			(val) =>
				!Number.isNaN(Number(val)) && Number(val) >= 0 && Number(val) <= 360,
			{
				message: 'Angle must be between 0-360 degrees',
			},
		),
	angleY: z
		.string()
		.refine(
			(val) =>
				!Number.isNaN(Number(val)) && Number(val) >= 0 && Number(val) <= 360,
			{
				message: 'Angle must be between 0-360 degrees',
			},
		),
	angleZ: z
		.string()
		.refine(
			(val) =>
				!Number.isNaN(Number(val)) && Number(val) >= 0 && Number(val) <= 360,
			{
				message: 'Angle must be between 0-360 degrees',
			},
		),
});

export type RotateCameraData = z.infer<typeof rotateCameraSchema>;
