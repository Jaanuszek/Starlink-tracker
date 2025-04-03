import * as z from 'zod';

export const showTrajectorySchema = z.object({
	starlinkId: z.string().nonempty('ID jest wymagane'),
});

export type ShowTrajectoryData = z.infer<typeof showTrajectorySchema>;
