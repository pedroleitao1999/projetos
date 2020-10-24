<template>
    <div class="container">
        <h2>Clarify Students Question</h2>
        <v-container class="create-buttons">
            <v-container>
                <p>Write clarification request id</p>
                <input v-model="message">
            </v-container>
            <v-container>
                <p class="pl-0">Set response</p>
                <input v-model="message">
            </v-container>
            <v-container>
                <v-btn @click="createClarification" depressed color="primary">
                    Create clarification
                </v-btn>
            </v-container>
        </v-container>
    </div>
</template>

<script>
    import { Component, Vue } from 'vue-property-decorator';
    import StatementManager from '@/models/statement/StatementManager';
    import Assessment from '@/models/management/Assessment';
    import RemoteServices from '@/services/RemoteServices';

    @Component
    export default class CreateClarificationRequestView extends Vue {

        statementManager: StatementManager = StatementManager.getInstance;
        availableAssessments: Assessment[] = [];

        async created() {
            await this.$store.dispatch('loading');
            this.statementManager.reset();
            try {
                this.availableAssessments = await RemoteServices.getAvailableAssessments();
            } catch (error) {
                await this.$store.dispatch('error', error);
            }
            await this.$store.dispatch('clearLoading');
        }

        async createClarification() {
            try {
                await this.statementManager.getClarificationStatement();
                await this.$router.push({ name: 'solve-clarification' });
            } catch (error) {
                await this.$store.dispatch('error', error);
            }
        }

    };
</script>

<style scoped>

</style>