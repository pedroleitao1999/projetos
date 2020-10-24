import ClarificationRequest from '@/models/statement/StatementClarificationRequest';
import User from '@/models/user/User';

export default class StatementClarification {
    id!: number;
    description!: string;
    teacher!: User;
    request!: ClarificationRequest;

    constructor(jsonObj?: StatementClarification) {
        if (jsonObj) {
            this.id = jsonObj.id;
            this.description = jsonObj.description;
            this.teacher = jsonObj.teacher;
            this.request = jsonObj.request;
        }
    }
}
