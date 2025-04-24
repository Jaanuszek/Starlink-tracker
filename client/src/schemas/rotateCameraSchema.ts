import * as z from 'zod';

export const rotateCameraSchema = z.object({
	angleX: z.preprocess(
		(val) => {
			if (val === '') return undefined;
			return Number(val);
		},
		z
			.number({
				required_error: 'Angle around x-axis is required',
				invalid_type_error: 'Angle must be a valid number',
			})
			.min(-360, 'Angle must be between -360 and 360 degrees')
			.max(360, 'Angle must be between -360 and 360 degrees'),
	),

	angleY: z.preprocess(
		(val) => {
			if (val === '') return undefined;
			return Number(val);
		},
		z
			.number({
				required_error: 'Angle around y-axis is required',
				invalid_type_error: 'Angle must be a valid number',
			})
			.min(-360, 'Angle must be between -360 and 360 degrees')
			.max(360, 'Angle must be between -360 and 360 degrees'),
	),
});

export type RotateCameraData = z.infer<typeof rotateCameraSchema>;
