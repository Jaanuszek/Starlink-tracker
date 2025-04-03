import { zodResolver } from '@hookform/resolvers/zod';
import { useForm } from 'react-hook-form';

import {
	DisableStarlinkData,
	disableStarlinkSchema,
} from '@/schemas/disableStarlinkSchema';
import { useDisableStarlink } from '@/mutations/useDisableStarlink';
import { Card, CardContent } from './ui/card';
import { TabsContent } from './ui/tabs';
import { Form, FormControl, FormField, FormItem, FormMessage } from './ui/form';
import { Input } from './ui/input';
import { Button } from './ui/button';

export const DisableStarlinkTab = () => {
	const form = useForm({
		resolver: zodResolver(disableStarlinkSchema),
		defaultValues: { starlinkId: '' },
	});

	const { mutateAsync: disableStarlink, isPending } = useDisableStarlink();

	const handleSubmit = async (data: DisableStarlinkData) => {
		await disableStarlink(data.starlinkId);
	};

	return (
		<TabsContent value='disable'>
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
								Hide Starlink
							</Button>
						</form>
					</Form>
				</CardContent>
			</Card>
		</TabsContent>
	);
};
