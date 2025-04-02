import { zodResolver } from '@hookform/resolvers/zod';
import { useForm } from 'react-hook-form';

import {
	ShowTrajectoryData,
	showTrajectorySchema,
} from '@/schemas/showTrajectorySchema';
import { useShowTrajectory } from '@/mutations/useShowTrajectory';
import { Card, CardContent } from './ui/card';
import { TabsContent } from './ui/tabs';
import { Form, FormControl, FormField, FormItem, FormMessage } from './ui/form';
import { Input } from './ui/input';
import { Button } from './ui/button';

export const ShowTrajectoryTab = () => {
	const form = useForm({
		resolver: zodResolver(showTrajectorySchema),
		defaultValues: { starlinkId: '' },
	});

	const { mutateAsync: showTrajectory, isPending } = useShowTrajectory();

	const handleSubmit = async (data: ShowTrajectoryData) => {
		await showTrajectory(data.starlinkId);
	};

	return (
		<TabsContent value='trajectory'>
			<Card>
				<CardContent>
					<Form {...form}>
						<form onSubmit={form.handleSubmit(handleSubmit)}>
							<FormField
								control={form.control}
								name='starlinkId'
								render={({ field }) => (
									<FormItem>
										<FormControl>
											<Input {...field} placeholder='Enter ID, f.e. 123' />
										</FormControl>
										<FormMessage />
									</FormItem>
								)}
							/>
							<Button disabled={isPending} type='submit' className='mt-2'>
								Show Trajectory
							</Button>
						</form>
					</Form>
				</CardContent>
			</Card>
		</TabsContent>
	);
};
